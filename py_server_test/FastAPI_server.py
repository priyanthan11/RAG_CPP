from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

class EmbRequest(BaseModel):
    text: str

@app.post("/embed")
def embed(req: EmbRequest):
    # Dummy embedding: convert each char to float
    embedding = [float(ord(c)) for c in req.text]
    return {"embedding": embedding}
