open Types;

open ReasonJs;

external sillyname : unit => string = "" [@@bs.module];
external shortId : unit => string = "shortid" [@@bs.module];

type state = {
  users: Js.Array.t user,
  messages: Js.Array.t message,
  channels: Js.Array.t channel,
  activeChannel: channelId,
  activeUser: user
};

let component = ReasonReact.statefulComponent "Root";

let messageId = ref 0;

let createMessage messageText user channel => {
  messageId := !messageId + 1;
  {id: !messageId, author: user, channelId: channel, content: messageText}
  };

let getChannelById channelId channels =>
  Js.Array.find (fun channel => channel.id == channelId) channels;

  let handleNewMessage messageText {ReasonReact.state: state} => {
  Socket.emit socket Socket.messageEvent messageText;
    let messages =
      Js.Array.concat
      [|createMessage messageText state.activeUser state.activeChannel|] state.messages;
  ReasonReact.Update {...state, messages}
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
  {
      users: [|activeUser|],
      messages: [||],
      channels: [|
        {id: 1, name: "general"},
        {id: 2, name: "react"},
        {id: 3, name: "reasonml"},
        {id: 4, name: "another-fake-channel"}
      |],
      activeChannel: 2,
      activeUser
    }
    },
    render: fun self => {
      let foo = self.update handleNewMessage;
      let activeChannel = getChannelById self.state.activeChannel self.state.channels;
      let activeMessages = filterMessagesByChannel self.state.messages self.state.activeChannel;
      <div className="app-container">
        <ChannelList
          onChannelSelect=(self.update handleChannelChange)
          channels=self.state.channels
        active=self.state.activeChannel
      />
        <Chat channel=activeChannel messages=activeMessages onNewMessage=foo />
      </div>
  }
};