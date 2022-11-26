import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query history database for user portifolio
    portifolio = db.execute(
        "SELECT symbol, name, SUM(amount) AS shares FROM history WHERE user_id = ? GROUP BY symbol HAVING SUM(amount) > 0", session["user_id"])

    # Format list with "GROUP BY symbol" plus stock current price and total price
    for stock in portifolio:
        stock["price"] = lookup(stock["symbol"])["price"]
        stock["total"] = lookup(stock["symbol"])["price"] * stock["shares"]

    # Query users database for user cash
    cash = db.execute(
        "SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # Sum user assets
    total = 0
    for stock in portifolio:
        total += stock["total"]

    return render_template("index.html", cash=cash, portifolio=portifolio, total=total), 200


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)

        # Ensure share was submitted
        elif not request.form.get("shares"):
            return apology("must provide shares", 400)

        # Ensure numberic shares was submitted
        if not request.form.get("shares").isnumeric():
            return apology("must provide numeric share", 400)

        # Ensure positive integer shares grater than zero share was submitted
        elif not int(float(request.form.get("shares"))) > 0:
            return apology("must provide positive integer shares grater than zero", 400)

        # Query API for stock symbol
        stock = lookup(request.form.get("symbol"))

        # Ensure query isn't None
        if stock == None:
            return apology("stock not found", 400)

        # Query database for user cash
        user_cash = db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # Sum request value
        price = stock["price"]
        amount = float(request.form.get("shares"))
        purchase_value = price * amount

        # Ensure user has sufficient cash
        if not user_cash >= purchase_value:
            return apology("Insuficient cash", 403)

        # Withdraw purchase value from user cash
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?",
                   purchase_value, session["user_id"])

        # Register purchase into history table in database
        db.execute("INSERT INTO history (user_id, symbol, name, amount, price, type, datetime) VALUES (?, ?, ?, ?, ?, ?, ?) ",
                   session["user_id"], stock["symbol"], stock["name"], amount, price, "B", datetime.now())

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for user's history
    history = db.execute(
        "SELECT * FROM history WHERE user_id = ? ORDER BY datetime DESC", session["user_id"])

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/", code=200)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    stock = None

    if request.method == "POST":

        # Ensure stock was submitted
        if not request.form.get("symbol"):
            return apology("must provide a stock symbol", 400)

        # Query API for stock symbol
        stock = lookup(request.form.get("symbol"))

        # Ensure query isn't None
        if stock == None:
            return apology("stock not found", 400)

    # Render quote page whether stock is None or not
    return render_template("quote.html", stock=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        # Ensure password fields match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password mismatch", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure user doesn't exist already
        if len(rows) != 0:
            return apology("username already exists", 400)

        # Register the user
        hash = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?) ",
                   request.form.get("username"), hash)

        # Redirect user to home page
        return redirect("/", code=200)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure stock was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)

        # Ensure share was submitted
        elif not request.form.get("shares"):
            return apology("must provide shares", 400)

        # Ensure numberic shares was submitted
        if not request.form.get("shares").isnumeric():
            return apology("must provide numeric share", 400)

        # Ensure positive integer shares grater than zero share was submitted
        elif not int(float(request.form.get("shares"))) > 0:
            return apology("must provide positive integer shares grater than zero", 400)

        # Query API for stock symbol
        stock = lookup(request.form.get("symbol"))

        # Ensure query isn't None
        if stock == None:
            return apology("stock not found", 400)

        # Query history database for user's portifolio
        portifolio = db.execute(
            "SELECT symbol, SUM(amount) AS shares FROM history WHERE user_id = ? AND symbol = ? GROUP BY symbol", session["user_id"], stock["symbol"])

        # Ensure user has sufficient shares to sell
        if int(float(request.form.get("shares"))) > portifolio[0]["shares"]:
            return apology("you don't own that many shares of the stock", 400)

        # Sum request value
        price = stock["price"]
        amount = float(request.form.get("shares"))
        sale_value = price * amount

        # Withdraw purchase value from user cash
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   sale_value, session["user_id"])

        # Register purchase into history table in database
        db.execute("INSERT INTO history (user_id, symbol, name, amount, price, type, datetime) VALUES (?, ?, ?, ?, ?, ?, ?) ",
                   session["user_id"], stock["symbol"], stock["name"], amount * -1, price, "S", datetime.now())

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # Query database for user's portifolio
        portifolio = db.execute(
            "SELECT symbol FROM history WHERE user_id = ? GROUP BY symbol HAVING SUM(amount) > 0", session["user_id"])

        return render_template("sell.html", portifolio=portifolio)


@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    """Change profile info"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure cash was submitted
        if not request.form.get("cash"):
            return apology("must provide new cash", 400)

        # Ensure numberic cash was submitted
        if not request.form.get("cash").isnumeric():
            return apology("must provide numeric cash", 400)

        # Ensure positive float cash grater than zero share was submitted
        elif not float(request.form.get("cash")) > 0:
            return apology("must provide positive float cash grater than zero", 400)

        # Update user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   request.form.get("cash"), session["user_id"])

        return redirect("/settings")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        # Query database for user's info
        user = db.execute(
            "SELECT username, cash FROM users WHERE id = ?", session["user_id"])[0]

        return render_template("settings.html", user=user)
