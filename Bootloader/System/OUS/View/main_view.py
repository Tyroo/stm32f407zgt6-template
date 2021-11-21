import os
from tkinter import (ttk, Tk, N, W, E, S, Y, FALSE, VERTICAL,
                     PhotoImage, StringVar,
                     filedialog, Text, TclError)

CURRENT_PATH = os.getcwd()


class MainViewModel(object):
    """
    主视图模型类，包含主视图数据集
    """
    def __init__(self, model):
        # 初始化属性
        self.model = model
        self.feet = StringVar()
        self.file = None
        self.file_name = None

    def mSetData(self, key):
        pass

    def mGetData(self, key):
        pass

    def build(self):
        pass


class MainViewEvent(MainViewModel):
    """
    主视图事件类，包含主视图中控件的事件回调函数
    """
    def __init__(self, event, model):
        # 初始化属性
        self.event = event
        super(MainViewEvent, self).__init__(model)  # 父类MainViewEvent初始化

    def eStartUart(self):
        pass

    def eOpenFile(self):

        try:
            self.file = filedialog.askopenfile(filetypes=[('二进制文件', '*.bin')])
            self.file_name = self.file.name
        except AttributeError as AE:
            self.file_name = ''
        self.feet.set(self.file_name)

    def eStartUpdate(self):

        if not self.file_name:
            self.feet.set('请选择升级文件...')
            return
        self.feet.set('正在开始升级...')

    def build(self):
        pass


class MainViewFrame(MainViewEvent):
    """
    主视图视图框架，包含组成主视图的各个控件
    """
    def __init__(self, win: Tk,
                 event: MainViewEvent,
                 model: MainViewModel):
        # 属性初始化
        self.win = win
        self.frame = ttk.Frame(self.win, padding="10 10 10 10")
        self.style = ttk.Style()

        # 导入主题（awdark）
        self.win.tk.call('lappend', 'auto_path', './Resource/theme/awthemes-10.4.0')
        self.win.tk.call('package', 'require', 'awdark')

        # 实例化父类
        MainViewEvent.__init__(self, self.event, self.model)  # 父类MainViewEvent初始化

    def fSetWinStyle(self, title: str, theme: str, icon_path: str):
        # 设置窗口标题
        self.win.title(title)

        try:
            # 设置窗口图标
            self.win.tk.call('wm', 'iconphoto', self.win.w,
                             PhotoImage(file=icon_path))
            # 设置窗口主题
            self.style.theme_use(theme)
        except TclError as TE:
            pass
        # 设置框架属性
        self.win.columnconfigure(0, weight=1)
        self.win.rowconfigure(0, weight=1)

    def fSetFrameStyle(self):
        # 设置Frame样式
        self.frame.grid(column=0, row=0, sticky=(N, W, E, S))

    def fAddElement(self):
        # 创建菜单
        # self.win.option_add('*tearOff', FALSE)
        # menubar = Menu(self.win)
        # self.win['menu'] = menubar
        # menu_file = Menu(menubar)
        # menu_edit = Menu(menubar)
        # menubar.add_cascade(menu=menu_file, label='File')
        # menubar.add_cascade(menu=menu_edit, label='Edit')
        # 增加输入框
        ttk.Entry(self.frame, textvariable=self.feet) \
            .grid(column=1, row=1, sticky=(W, E))
        # 增加按钮
        ttk.Button(self.frame, text='打开文件', command=self.eOpenFile) \
            .grid(column=2, row=1, sticky=W, padx=5)
        # 增加按钮
        ttk.Button(self.frame, text='开始升级', command=self.eStartUpdate) \
            .grid(column=3, row=1, sticky=W, padx=5)
        # 增加提示框
        ttk.Label(self.frame, width=40, textvariable=self.feet) \
            .grid(column=1, row=2, sticky=(W, N))

        ReadText = Text(self.frame, width=50, height=10)
        ReadText.grid(column=1, row=3, sticky=(W, N, E, S))
        ReadScroll = ttk.Scrollbar(self.frame, orient=VERTICAL, command=ReadText.yview)
        ReadScroll.grid(column=2, row=3, sticky=(S, N, W))
        ReadText['yscrollcommand'] = ReadScroll.set

        SendText = Text(self.frame, width=50, height=10)
        SendText.grid(column=1, row=4, sticky=(W, N, E, S))
        SendScroll = ttk.Scrollbar(self.frame, orient=VERTICAL, command=SendText.yview)
        SendScroll.grid(column=2, row=4, sticky=(S, N, W))
        SendText['yscrollcommand'] = SendScroll.set

    def build(self):
        # 开始构建框架对象
        self.fSetWinStyle('OUS', 'awdark', './Resource/icon/ironcat.png')
        self.fSetFrameStyle()
        self.fAddElement()


class MainView(MainViewFrame):
    """
    主窗口类， 用于构建主窗口视图
    """
    def __init__(self, win: Tk,
                 model: MainViewModel = None,
                 event: MainViewEvent = None):
        # 创建主窗口对象
        self.win = win
        self.model = model
        self.event = event

        # 示例化父类MainViewFrame
        MainViewFrame.__init__(self, self.win, self.event, self.model)
        MainViewFrame.build(self)

    # 构建MainView对象
    def build(self, debug=False):
        self.win.mainloop()

