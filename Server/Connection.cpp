#include "Connection.h"

void Connection::Send(NetworkMessage& msg, bool reliable)
{
	msg.PrepareSend();
	Socket->send_to(asio::buffer(msg.Body), RemoteEndpoint);
}
