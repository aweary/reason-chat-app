open Types;

open Bs_fetch;

open ReasonJs;

external sillyname : unit => string = "" [@@bs.module];

external shortId : unit => string = "shortid" [@@bs.module];

type state = {
  users: Js.Array.t user,
  messages: Js.Array.t message2,
  channels: Js.Array.t channel,
  activeChannel: channelId,
  activeUser: user,
  loggedIn: bool,
  joinError: option string
};

let socket = Socket.createClient Api.apiHost {"query": {"token": "foo"}};

let component = ReasonReact.statefulComponent "Root";

Js.log Api.apiHost;

let fetchMessageHistory callback =>
  Js.Promise.(
    fetch (Api.apiHost ^ "api/messages?limit=50") |> then_ Response.json |>
    then_ (
      fun json => {
        switch (Js.Json.decodeArray json) {
        | Some foo => callback (Js.Array.map Api.decodeMessage2 foo)
        | None => Js.log "Received message response that does not match expected format"
        };
        resolve ()
      }
    )
  );

let updateUserList users {ReasonReact.state: state} =>
  ReasonReact.Update {...state, users: Js.Array.concat users state.users};

let updateMessageHistory messages {ReasonReact.state: state} =>
  ReasonReact.Update {...state, messages: Js.Array.concat messages state.messages};

let createMessage messageText (user: user) channel => {
  id: shortId (),
  author: {
    nickname: user.nickname,
    id: user.id,
    /* TODO unify these field names */
    avatar: user.picture
  },
  channel,
  content: messageText,
  timestamp: Js.Date.toString (Js.Date.make ())
};

let createUser nickname => {
    firstName: "",
    lastName: "",
    nickname: nickname,
    id: shortId (),
    status: Available,
    picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
    roles: []
};

let getChannelById (channelId: channelId) channels =>
  Js.Array.find (fun (channel: channel) => channel.id == channelId) channels;

let handleNewLocalMessage messageText {ReasonReact.state: state} => {
  let message = createMessage messageText state.activeUser state.activeChannel;
  Socket.emitNewMessage socket message;
  let messages = Js.Array.concat [|message|] state.messages;
  ReasonReact.Update {...state, messages}
};

let handleNewRemoteMessage messageJson {ReasonReact.state: state} => {
  let message = Api.decodeMessage2 (Js.Json.parseExn messageJson);
  if (message.author.id != state.activeUser.id) {
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

let onNewUserJoinSuccess nickname {ReasonReact.state: state} => { 
  let user = createUser nickname;
  ReasonReact.Update {...state, loggedIn: true, activeUser: user }
};

let onNewUserJoinFailure errorMessage {ReasonReact.state: state} => {
  ReasonReact.Update {...state, joinError: Some errorMessage }
};

let handleNewUserJoin onSuccess onFailure username => {
  let user = {
    firstName: "",
    lastName: "",
    nickname: username,
    id: shortId (),
    status: Available,
    picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
    roles: []
  };
  Api.registerUser username "fakepassword" onSuccess onFailure
};

let handleChannelChange channelId {ReasonReact.state: state} =>
  ReasonReact.Update {...state, activeChannel: channelId};

let filterMessagesByChannel messages channelId =>
  Js.Array.filter (fun message => message.channel == channelId) messages;

let make _children => {
  ...component,
  initialState: fun _ => {
    let token = SafeStorage.readTokenFromStorage ();
    /* Api.authenticate "foobar" "baz"; */
    /* @TODO stop generating a fake user */
    let tokenExists = token !== "";
    let activeUser = {
      firstName: "",
      lastName: "",
      nickname: tokenExists ? token : sillyname (),
      id: shortId (),
      status: Available,
      picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
      roles: []
    };
    /* Socket.emitNewUser socket activeUser; */
    {
      users: [|activeUser|],
      messages: [||],
      channels: [|
        {id: 1, name: "general", topic: "General discussions."},
        {id: 2, name: "react", topic: "Lets talk about React!"},
        {id: 3, name: "reasonml", topic: "Reasonable discussions."},
        {id: 4, name: "another-fake-channel", topic: "Why even write this?"}
      |],
      activeChannel: 1,
      activeUser,
      loggedIn: tokenExists,
      joinError: None
    }
  },
  didMount: fun self => {
    let fetchUsers = self.update updateUserList;
    let fetchMessages = self.update updateMessageHistory;
    fetchMessageHistory fetchMessages;
    Socket.on socket Socket.messageEvent (self.update handleNewRemoteMessage);
    /* Socket.on socket Socket.userEvent (self.update handleNewRemoteUser); */
    ReasonReact.NoUpdate
  },
  render: fun self => {
    let boundonNewUserJoinSuccess = self.update onNewUserJoinSuccess;
    let boundonNewUserJoinFailure = self.update onNewUserJoinFailure;
    if (not self.state.loggedIn) {
      <Login joinError=self.state.joinError onJoinAttempt=(handleNewUserJoin boundonNewUserJoinSuccess boundonNewUserJoinFailure) />
    } else {
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
          onNewMessage=(self.update handleNewLocalMessage)
        />
      </div>
    }
  }
};