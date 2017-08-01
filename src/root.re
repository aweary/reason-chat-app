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
  authenticating: bool,
  joinError: option string,
  socket: option Socket.socket,
  token: string
};

let component = ReasonReact.statefulComponent "Root";

Js.log Api.apiHost;

let updateUserList users {ReasonReact.state: state} =>
  ReasonReact.Update {...state, users: Js.Array.concat users state.users};

let updateMessageHistory json {ReasonReact.state: state} => {
  let messages =
    switch (Js.Json.decodeArray json) {
    | Some decodedJson => Js.Array.map Api.decodeMessage2 decodedJson
    | None => Js.Exn.raiseError "Unable to parse message history."
    };
  /* @TODO should we try to merge existing local messages? */
  ReasonReact.Update {...state, messages}
};

/* ReasonReact.Update {...state, messages: Js.Array.concat messages state.messages}; */
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
  nickname,
  id: shortId (),
  status: Available,
  picture: "https://semantic-ui.com/images/avatar2/large/matthew.png",
  roles: []
};

let getChannelById (channelId: channelId) channels =>
  Js.Array.find (fun (channel: channel) => channel.id == channelId) channels;

let handleNewLocalMessage messageText {ReasonReact.state: state} =>
  switch state.socket {
  | Some socket =>
    let message = createMessage messageText state.activeUser state.activeChannel;
    Js.log message;
    /* @TODO pass token for all messages */
    Socket.emitNewMessage socket message;
    let messages = Js.Array.concat [|message|] state.messages;
    ReasonReact.Update {...state, messages}
  | None => Js.Exn.raiseError "Received local message without a socket instance."
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

let onNewUserJoinFailure errorMessage {ReasonReact.state: state} =>
  ReasonReact.Update {...state, joinError: Some errorMessage};

let handleNewUserJoin onSuccess onFailure username password =>
  Api.registerUser username password onSuccess onFailure;
let handleUserLogin onSuccess onFailure username password =>
  Api.authenticateUser username password onSuccess onFailure;

let handleChannelChange channelId {ReasonReact.state: state} =>
  ReasonReact.Update {...state, activeChannel: channelId};

let filterMessagesByChannel messages channelId =>
  Js.Array.filter (fun message => message.channel == channelId) messages;

let connectToServer self token => {
  Api.authenticatedFetch
    ::token
    path::"api/messages?limit=50"
    method_::Get
    onSuccess::(self.ReasonReact.update updateMessageHistory)
    onFailure::(fun res => Js.log res)
    ();
  let socket = Socket.createClient Api.apiHost {"query": {"token": token}};
  Socket.on socket Socket.messageEvent (self.ReasonReact.update handleNewRemoteMessage);
  socket
};

let onNewUserJoinSuccess self response {ReasonReact.state: state} => {
  let token = response##token;
  let user = createUser response##username;
  let socket = connectToServer self token;
  SafeStorage.writeTokenToStorage token;
  ReasonReact.Update {...state, token, loggedIn: true, activeUser: user, socket: Some socket}
};

let onTokenValidationSuccess self token json {ReasonReact.state: state} => {
  Js.log "It worked!";
  let response = Api.jsonToObject json;
  let user = createUser response##username;
  let socket = connectToServer self token;
  ReasonReact.Update {
    ...state,
    authenticating: false,
    loggedIn: true,
    activeUser: user,
    socket: Some socket
  }
};

let make _children => {
  ...component,
  initialState: fun self => {
    let token = SafeStorage.readTokenFromStorage ();
    let hasExistingToken = token !== "";
    /* Api.authenticate "foobar" "baz"; */
    /* @TODO stop generating a fake user */
    let user = createUser "";
    /* Socket.emitNewUser socket activeUser; */
    {
      users: [|user|],
      messages: [||],
      channels: [|
        {id: 1, name: "general", topic: "General discussions."},
        {id: 2, name: "react", topic: "Lets talk about React!"},
        {id: 3, name: "reasonml", topic: "Reasonable discussions."},
        {id: 4, name: "another-fake-channel", topic: "Why even write this?"}
      |],
      activeChannel: 1,
      activeUser: user,
      loggedIn: false,
      authenticating: hasExistingToken,
      token,
      joinError: None,
      socket: None
    }
  },
  didMount: fun self =>
    if self.state.authenticating {
      Js.log "Validating that topen...";
      Api.validateToken
        token::self.state.token
        onSuccess::(self.update (onTokenValidationSuccess self self.state.token))
        onFailure::(fun x => Js.log x)
        ();
      ReasonReact.NoUpdate
    } else {
      ReasonReact.NoUpdate
    },
  render: fun self => {
    let boundonNewUserJoinSuccess = self.update (onNewUserJoinSuccess self);
    let boundonNewUserJoinFailure = self.update onNewUserJoinFailure;
    Js.log self.state.loggedIn;
    if (not self.state.loggedIn) {
      if self.state.authenticating {
        <span> (ReasonReact.stringToElement "Loading...") </span>
      } else {
        <LoginOrRegister
          onSignUp=(handleNewUserJoin boundonNewUserJoinSuccess boundonNewUserJoinFailure)
          onLogin=(handleUserLogin boundonNewUserJoinSuccess boundonNewUserJoinFailure)
        />
      }
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