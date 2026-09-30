import turtle
import re

t = turtle.Turtle()
t.speed(0)

with open("instructions", "r") as f:
    for ligne in f:
        ligne = ligne.strip()

        if not ligne:
            continue

        # Avance X spaces
        m = re.match(r"Avance (\d+) spaces", ligne)
        if m:
            t.forward(int(m.group(1)))
            continue

        # Recule X spaces
        m = re.match(r"Recule (\d+) spaces", ligne)
        if m:
            t.backward(int(m.group(1)))
            continue

        # Tourne gauche de X degrees
        m = re.match(r"Tourne gauche de (\d+) degrees", ligne)
        if m:
            t.left(int(m.group(1)))
            continue

        # Tourne droite de X degrees
        m = re.match(r"Tourne droite de (\d+) degrees", ligne)
        if m:
            t.right(int(m.group(1)))
            continue

        print("Instruction inconnue :", ligne)

turtle.done()