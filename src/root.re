open Types;

open ReasonJs;

type state = {
  messages: Js.Array.t message,
  channels: Js.Array.t channel,
  activeChannel: channelId
};

let component = ReasonReact.statefulComponent "Root";

let make _children => {
  let getChannelById channelId channels => {
    Js.Array.find (fun channel => channel.id == channelId) channels
  };
  let handleNewMessage messageText {ReasonReact.state: state} => {
    let messages =
      Js.Array.concat
        [|
          /* TODO use real author, channel, id */
          {id: 22, author: 1, content: messageText, channelId: state.activeChannel}
        |]
        state.messages;
    ReasonReact.Update {...state, messages: messages}
  };
  let handleChannelChange channelId {ReasonReact.state : state} => {
    ReasonReact.Update { ...state, activeChannel: channelId }
  };
  let filterMessagesByChannel messages channelId => {
    Js.Array.filter (fun message => message.channelId == channelId) messages
  };
  {
    ...component,
    initialState: fun _ => {
      messages: [||],
      channels: [|
        { id: 1, name: "general" },
        { id: 2, name: "react" },
        { id: 3, name: "reasonml" },
        { id: 4, name: "another-fake-channel" },
      |],
      activeChannel: 2
    },
    render: fun self => {
      let foo = self.update handleNewMessage;
      let activeChannel = getChannelById self.state.activeChannel self.state.channels;
      let activeMessages = filterMessagesByChannel self.state.messages self.state.activeChannel;
      <div className="app-container">
        <ChannelList
          onChannelSelect=(self.update handleChannelChange)
          channels=self.state.channels
          active=self.state.activeChannel />
        <Chat channel=activeChannel messages=activeMessages onNewMessage=foo />
      </div>
    }
  }
};