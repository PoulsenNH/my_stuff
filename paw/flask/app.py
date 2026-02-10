from flask import Flask, render_template
app = Flask(__name__)

@app.route("/")

def main():
    print("Main function started")
    return render_template("index.html")

def generate_top_bar():
    print("Generate Top Bar function started")
