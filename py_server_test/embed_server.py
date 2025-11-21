
from sentence_transformers import SentenceTransformer
from fastapi import FastAPI, Request
from pydantic import BaseModel
from typing import List
import uvicorn

app = FastAPI()


model = SentenceTransformer("Snowflake/snowflake-arctic-embed-m")


class EmbedRequest(BaseModel):
    text: str


@app.post("/embed")
async def embed(req: EmbedRequest):
    embedding = model.encode([req.text], normalize_embeddings=True)
    return {"embedding": embedding[0].tolist()}

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=11435)
