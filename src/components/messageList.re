open Types;

type state = {messageRef: option Dom.element};

let component = ReasonReact.statefulComponent "MessageList";

let mapMessageToListItem message => <ChatMessage key=message.id message />;

let scrollToBottom state =>
  switch state.messageRef {
  | Some node => ElementRe.setScrollTop node (ElementRe.scrollHeight node)
  | None => ()
  };

let make ::messages _ => {
  let setMessageRef theRef _ => ReasonReact.SilentUpdate {messageRef: Js.Null.to_opt theRef};
  {
    ...component,
    initialState: fun _ => {messageRef: None},
    didMount: fun self => {
      scrollToBottom self.state;
      ReasonReact.NoUpdate
    },
    didUpdate: fun {newSelf} => {
      scrollToBottom newSelf.state;
    },
    render: fun self =>
      <div ref=(self.update setMessageRef) className="chat--container">
        <ul className="chat--content">
          (ReasonReact.arrayToElement (Js.Array.map mapMessageToListItem messages))
        </ul>
      </div>
  }
};