type userStatus =
  | Available
  | Idle
  | DoNotDisturb
  | Invisible;

type userId = string;

type messageId = string;

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
  id: channelId,
  topic: string
};


 type messageAuthor = {
  nickname: string,
  id: userId,
  avatar: string
};

type message2 = {
  content: string,
  id: messageId,
  channel: channelId,
  timestamp: string,
  author: messageAuthor
}; 