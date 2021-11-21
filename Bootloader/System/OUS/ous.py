from View.main_view import MainView
from tkinter import Tk


if __name__ == '__main__':
    mw = Tk()
    mv = MainView(mw)
    mv.build(debug=False)




