from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html') 

@app.route('/home')
def home():
    return render_template('main.html')  

@app.route('/settings')
def settings():
    return render_template('settings.html') 
