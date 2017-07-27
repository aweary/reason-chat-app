open Types;

type state = {messageRef: option Dom.element};

let component = ReasonReact.statefulComponent "MessageList";


let mapMessageToListItem message => <ChatMessage key=message.id message />;

let make ::messages _ => {
  let setMessageRef theRef _ => ReasonReact.SilentUpdate {messageRef: Js.Null.to_opt theRef};
  {
  ...component,
  initialState: fun _ => { messageRef: None },
  didUpdate: fun {newSelf} => {
    switch newSelf.state.messageRef {
    | Some node => {
      ElementRe.setScrollTop node (ElementRe.scrollHeight node);
    }
    | None => ()
    }
  },
  render: fun self => {
    <div ref=(self.update setMessageRef) className="chat--container">
      <ul className="chat--content">
        (ReasonReact.arrayToElement (Js.Array.map mapMessageToListItem messages))
      </ul>
    </div>
  }
};
}