open Types;

type state = {messageRef: option Dom.element};

let component = ReasonReact.statefulComponent "MessageList";


/**
 * Map an array of message records to an unordered
 * list.
 */

let findAuthorForMessage message users => {
  switch (Js.Array.find (fun (user: user) => user.id == message.author) users) {
  | Some user => user
  | None => Js.Exn.raiseError "Message without user"
  }
};

let makeMessageMapper users message => {
  let author = findAuthorForMessage message users;
  <ChatMessage message author />
};

let make ::messages ::users _ => {
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
    let mapMessageToListItem = makeMessageMapper users;
    Js.log messages;
    <div ref=(self.update setMessageRef) className="chat--container">
      <ul className="chat--content">
        (ReasonReact.arrayToElement (Js.Array.map mapMessageToListItem messages))
      </ul>
    </div>
  }
};
}