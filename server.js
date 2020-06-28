const express = require("express");
const app = express();
const { PORT, mongoUri } = require("./config");
const mongoose = require("mongoose");
const cors = require("cors");
const morgan = require("morgan");
const bodyParser = require("body-parser");
const bucketListItemRoutes = require("./routes/api/bucketListItems");
const path = require("path");

app.use(cors()); // CORS 미들웨어
app.use(morgan("tiny")); // 요청에 대한 log를 남겨주는 미들웨어
app.use(bodyParser.json()); // POST 요청에 대한 body 값을 받아올 수 있게 해주는 미들웨어

mongoose
  .connect(mongoUri, {
    useNewUrlParser: true,
    useCreateIndex: true,
    useUnifiedTopology: true,
    useFindAndModify: false,
  })
  .then(() => console.log("MongoDB database Connected..."))
  .catch((err) => console.log(err));

app.use("/api/bucketListItems", bucketListItemRoutes);

// 배포시 build 된 파일들을 돌리기 위한 코드
if (process.env.NODE_ENV === "production") {
  app.use(express.static("client/dist"));
  app.get("*", (req, res) => {
    res.sendFile(path.resolve(__dirname, "client", "dist", "index.html"));
  });
}

app.listen(PORT, () =>
  console.log(`app listening at http://localhost:${PORT}`)
);
