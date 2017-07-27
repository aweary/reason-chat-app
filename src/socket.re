open Types;

type socket;
external createClient : string => Js.t {..} => socket = "socket.io-client" [@@bs.module];
external emit : socket => string => string => unit = "emit" [@@bs.send];
external on : socket => string => (string => unit) [@bs.uncurry] => unit = "on" [@@bs.send];

let userEvent = "@user";
let messageEvent = "@message";

let emitNewMessage socket message => {
  emit socket messageEvent (Api.encodeMessage2 message);
};