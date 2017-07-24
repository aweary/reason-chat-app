type userStatus =
  | Available
  | Idle
  | DoNotDisturb
  | Invisible;

type userId = string;

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
  author: user,
  id: messageId,
  channelId: channelId
};

type channel = {
  name: string,
  id: channelId
};