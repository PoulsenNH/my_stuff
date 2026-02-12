from flask import Flask, render_template, request, redirect

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/home', methods=['GET', 'POST'])
def home():
    if request.method == 'POST':
        msg = request.form.get('broadcast')
        if msg:
            msg = msg.strip()[:200] 
            with open('broadcasts.txt', 'w') as f:
                f.write(msg)
        return redirect('/')

    broadcast_msg = "This is a generic broadcast that is visible to anybody to give you an idea on what it would look like."
    try:
        with open('broadcasts.txt', 'r') as f:
            lines = f.readlines()
            if lines:
                broadcast_msg = lines[0].strip() 
    except FileNotFoundError:
        pass
    return render_template('main.html', broadcast=broadcast_msg)

@app.route('/settings')
def settings():
    return render_template('settings.html')
