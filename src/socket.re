open Types;

type socket;
external createClient : string => socket = "socket.io-client" [@@bs.module];
external emit : socket => string => string => unit = "emit" [@@bs.send];
external on : socket => string => (string => unit) [@bs.uncurry] => unit = "on" [@@bs.send];

let socketAddress = "http://localhost:3000";
let userEvent = "@user";
let messageEvent = "@message";

let emitNewUser socket (user: user) => {
    emit socket userEvent (Api.encodeUser user);
};

let emitNewMessage socket (message: message) author => {
  emit socket messageEvent (Api.encodeMessage message author);
};