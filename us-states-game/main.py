import turtle
import pandas
import tkinter as tk

screen = turtle.Screen()
screen.title("U.S. States Game")

turtle.setup(width=800, height=600)

# imported image
image = "blank_states_img.gif"
screen.addshape(image)
turtle.shape(image)
print("Screen size:", turtle.screensize())

df = pandas.read_csv("50_states.csv")
all_states = df.state.to_list()

pen = turtle.Turtle()
pen.shape('circle')
pen.shapesize(stretch_wid=0.1, stretch_len=0.1, outline=1)
pen.hideturtle()
pen.penup()

count = 0
correct_answer = []

while count < 50:
    prompt = screen.textinput(title=f"{len(correct_answer)}/50 States Correct",
                              prompt="Guess a state").title()
    print("input:", prompt)
    if prompt == "Exit":
        missing_states = []
        for state in all_states:
            if state not in correct_answer:
                missing_states.append(state)
        new_data = pandas.DataFrame(missing_states)
        new_data.to_csv("states_to_study.csv")
        break

    if prompt in correct_answer:
        continue

    # Check existence
    if prompt in df["state"].values:
        count += 1
        correct_answer.append(prompt)

        # Get the coordination of the state
        x = df[df['state'] == prompt]['x'].values[0]
        y = df[df['state'] == prompt]['y'].values[0]
        print(f"x: {x}, y: {y}")

        pen.goto(x, y)
        pen.write(prompt)
        pen.goto(0, 0)
        continue
    print("invalid answer")

# # Create a sub dataframe excluding the states in correct_answer
# states_to_study_df = df[~df['state'].isin(correct_answer)]
# # Save it as a csv file
# states_to_study_csv = states_to_study_df.to_csv("states_to_study.csv")
