open Types;

open ReasonJs;

external changeEventToObj : ReactEventRe.Form.t => Js.t {..} = "%identity";

type state = {value: string};

let component = ReasonReact.statefulComponent "Foo";

let make ::onNewMessage _children => {
  let handleChange event _ => {
    let target = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
    ReasonReact.Update {value: target##value}
  };
  let handleKeyDown event {ReasonReact.state: state} => {
    let keyCode = ReactEventRe.Keyboard.keyCode event;
    if (keyCode == 13 && not (ReactEventRe.Keyboard.shiftKey event)) {
      /* TODO check if shift key is being held */
      ReactEventRe.Keyboard.preventDefault event;
      if (state.value != "") {
        onNewMessage state.value
      };
      ReasonReact.Update {value: ""}
    } else {
      ReasonReact.NoUpdate
    }
  };
  {
    ...component,
    initialState: fun () => {value: ""},
    render: fun self =>
      <form className="message-input">
        <textarea
          value=self.state.value
          onKeyDown=(self.update handleKeyDown)
          onChange=(self.update handleChange)
        />
      </form>
  }
};