type userStatus =
  | Available
  | Idle
  | DoNotDisturb
  | Invisible;

type userId = int;

type messageId = int;

type channelId = int;

type user = {
  firstName: string,
  lastName: string,
  nickname: string,
  id: userId,
  status: userStatus,
  picture: string,
  roles: list string
};

type message = {
  content: string,
  author: userId,
  id: messageId,
  channelId: channelId
};

type channel = {
  name: string,
  id: channelId
};