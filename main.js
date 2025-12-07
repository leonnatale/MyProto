import net from "node:net";

const connection = net.createConnection({
    host: "localhost",
    port: 8080,
});

const message = [
    1, // major
    0, // minor
    1, // patch
    0, // method
];
// for (let i = 0; i < 509; i++) message.push(255);

connection.on("data", (msg) => {
    console.log(msg.toString());
});

connection.on("connect", () => {
    // connection.write(new Uint8Array(message));
    connection.write("abc");
});
