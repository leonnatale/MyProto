import net from "node:net";

const connection = net.createConnection({
    host: "localhost",
    port: 8080,
});

const message = [
    0, // major
    0, // minor
    1, // patch

    1, // method

    0, // add field
    ...Buffer.from("name\0"), // null terminated strings only
    1, // set field value
    ...Buffer.from("hi\0"),

    0, // add field
    ...Buffer.from("field_name\0"),
    1, // set field value
    ...Buffer.from("field_value\0"),
];

// for (let i = 0; i < 509; i++) message.push(255);

connection.on("data", (msg) => {
    console.log(msg.toString());
});

connection.on("connect", () => {
    connection.write(new Uint8Array(message));
    // connection.write();
});
