open Types;

type state = {messageRef: option Dom.element};

let component = ReasonReact.statefulComponent "MessageList";


/**
 * Map an array of message records to an unordered
 * list.
 */
let mapMessageToListItem (message: message) => <ChatMessage message />;

let make ::messages _ => {
  let setMessageRef theRef {ReasonReact.state} => ReasonReact.SilentUpdate {...state, messageRef: Js.Null.to_opt theRef};
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