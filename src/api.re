open Types;

let encodeMessage message (author: user) => {
  Json.Encode.(
    object_ [
      ("content", string message.content),
      ("id", string message.id),
      ("author", string author.id),
      ("channelId", int message.channelId)
    ]
  ) |> Js.Json.stringify
};

let encodeUser user => {
  Json.Encode.(
    object_ [
      ("nickname", string user.nickname),
      ("id", string user.id)
    ]
  ) |> Js.Json.stringify
};

let decodeUser userJson => {
  /* @TODO need to send all this info across the wire */
  Json.Decode.{
    firstName: "",
    lastName: "",
    nickname: userJson |> field "nickname" string,
    id: userJson |> field "id" string,
    status: Available,
    picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
    roles: []
  }
};


let decodeMessage messageJson => {
  Json.Decode.{
    content: messageJson |> field "content" string,
    id: messageJson |> field "id" string,
    author: messageJson |> field "author" string,
    channelId: messageJson |> field "channelId" int,
  }
}