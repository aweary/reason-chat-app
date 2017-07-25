open Types;
open Bs_fetch;
open ReasonJs;

external sillyname : unit => string = "" [@@bs.module];

external shortId : unit => string = "shortid" [@@bs.module];

let parseUserList : Js.Json.t => Js.Array.t Js.Json.t = [%bs.raw{|
  function parseUserList(userJson) {
    return userJson.map(function(user) {
      return JSON.parse(user)
    })
  }
|}];

type state = {
  users: Js.Array.t user,
  messages: Js.Array.t message,
  channels: Js.Array.t channel,
  activeChannel: channelId,
  activeUser: user
};

let socket = Socket.createClient Socket.socketAddress;

let component = ReasonReact.statefulComponent "Root";

let messageId = ref 0;

let fetchInitalUserList callback => {
  Js.Promise.(
    fetch "http://localhost:3000/api/users"
    |> then_ Response.json
    |> then_ (fun json => {
       callback (Js.Array.map Api.decodeUser (parseUserList json));
       resolve ()
    })
  )
};
let fetchMessageHistory callback => {
  Js.Promise.(
    fetch "http://localhost:3000/api/messages"
    |> then_ Response.json
    |> then_ (fun json => {
       callback (Js.Array.map Api.decodeMessage (parseUserList json));
       resolve ()
    })
  )
};

let updateUserList users {ReasonReact.state: state} => {
  ReasonReact.Update {...state, users: Js.Array.concat users state.users }
};

let updateMessageHistory messages {ReasonReact.state: state} => {
  ReasonReact.Update {...state, messages: Js.Array.concat messages state.messages }
};

let createMessage messageText user channel => {
  id: shortId (),
  author: user,
  channelId: channel,
  content: messageText
};

let getChannelById channelId channels =>
  Js.Array.find (fun channel => channel.id == channelId) channels;

let handleNewLocalMessage messageText {ReasonReact.state: state} => {
  /* Socket.emit socket Socket.messageEvent messageText; */
  let message = createMessage messageText state.activeUser.id state.activeChannel;
  Socket.emitNewMessage socket message state.activeUser;
  let messages = Js.Array.concat [|message|] state.messages;
  ReasonReact.Update {...state, messages}
};

let handleNewRemoteMessage messageJson {ReasonReact.state: state} => {
  let message = Api.decodeMessage (Js.Json.parseExn messageJson);
  if (message.author != state.activeUser.id) {
    let messages = Js.Array.concat [|message|] state.messages;
    ReasonReact.Update {...state, messages}
  } else {
    ReasonReact.NoUpdate
  }
};

let handleNewRemoteUser userJson {ReasonReact.state: state} => {
  let user = Api.decodeUser (Js.Json.parseExn userJson);
  if (user.id != state.activeUser.id) {
    let users = Js.Array.concat [|user|] state.users;
    ReasonReact.Update {...state, users}
  } else {
    ReasonReact.NoUpdate
  }
};

let handleChannelChange channelId {ReasonReact.state: state} =>
  ReasonReact.Update {...state, activeChannel: channelId};

let filterMessagesByChannel messages channelId =>
  Js.Array.filter (fun message => message.channelId == channelId) messages;

let make _children => {
  ...component,
  initialState: fun _ => {
    /* @TODO stop generating a fake user */
    let activeUser = {
      firstName: "",
      lastName: "",
      nickname: sillyname (),
      id: shortId (),
      status: Available,
      picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
      roles: []
    };
    Socket.emitNewUser socket activeUser;
    {
      users: [|activeUser|],
      messages: [||],
      channels: [|
        {id: 1, name: "general"},
        {id: 2, name: "react"},
        {id: 3, name: "reasonml"},
        {id: 4, name: "another-fake-channel"}
      |],
      activeChannel: 1,
      activeUser
    }
  },
  didMount: fun self => {
    let fetchUsers = self.update updateUserList;
    let fetchMessages = self.update updateMessageHistory;
    fetchInitalUserList fetchUsers;
    fetchMessageHistory fetchMessages;
    Socket.on socket Socket.messageEvent (self.update handleNewRemoteMessage);
    Socket.on socket Socket.userEvent (self.update handleNewRemoteUser);
    ReasonReact.NoUpdate
  },
  render: fun self => {
    let activeChannel = getChannelById self.state.activeChannel self.state.channels;
    let activeMessages = filterMessagesByChannel self.state.messages self.state.activeChannel;
    <div className="app-container">
      <ChannelList
        onChannelSelect=(self.update handleChannelChange)
        channels=self.state.channels
        active=self.state.activeChannel
      />
      <Chat
        channel=activeChannel
        messages=activeMessages
        users=self.state.users
        onNewMessage=(self.update handleNewLocalMessage)
      />
    </div>
  }
};