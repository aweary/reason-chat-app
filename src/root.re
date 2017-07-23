open Types;

type state = { messages: Js.Array.t message };
let component = ReasonReact.statefulComponent "Root";

let make _children => {
  let handleNewMessage messageText {ReasonReact.state} => {
    Js.log (messageText);
    Js.log (state.messages);
    let messages = Js.Array.concat [|
      { id: 22, author: 1, content: messageText }
    |] state.messages;
    ReasonReact.Update { messages: messages } 
  };
  {
    ...component,
    initialState: fun _ => { messages: [||] },
    render: fun self => {
      let foo = self.update handleNewMessage;
      <div className="app-container">
         <ChannelList /> 
        <Chat messages=self.state.messages onNewMessage=(foo) />
       </div>
    }
  }
}
