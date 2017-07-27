type state = { showMentionList: bool };
let component = ReasonReact.statefulComponent "Content";

let onMessageInputChange inputValue {ReasonReact.state: state} => {
  let length = Js.String.length inputValue;
  let lastChar = Js.String.get inputValue (length - 1);
  let secondToLastChar = Js.String.get inputValue (length - 2);
  if (lastChar == "@" && (length == 1 || secondToLastChar == " ")) {
    ReasonReact.Update { showMentionList: (not state.showMentionList) }
  } else if (state.showMentionList) {
    ReasonReact.Update { showMentionList: false }
  } else {
    ReasonReact.NoUpdate
  }
};

let make ::onNewMessage ::messages ::channel _ => {
  {
  ...component,
  initialState: fun _ => {
    showMentionList: false
  },
  render: fun self => {
    <div className="chat-content">
      <Toolbar channel />
      <MessageList messages />
      <MentionList show=self.state.showMentionList />
      <MessageInput onChange=(self.update onMessageInputChange) channel onNewMessage />
    </div>
  }
}
}