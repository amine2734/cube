
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qtoolbox.h>
#include <qframe.h>
#include <qprogressbar.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qtimer.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qregexp.h>

#include <iostream>
#include <cstdarg>

#include "gui.h"
#include "animator.h"

extern MainDlg  *Window;
extern Animator *AnimCube;
extern QApplication *Application;

GLBox::GLBox(int fps, QWidget *parent, const char *name):
		QGLWidget( parent, name )
{
	if ( !fps )
		gl_timer = 0;
	else
	{
		gl_timer = new QTimer( this );
		connect( gl_timer, SIGNAL(timeout()), this, SLOT(timerTick()) );
		gl_timer->start( 1000 / fps );
	}
}

void GLBox::mousePressEvent(QMouseEvent *event)
{
	if( Window->block >= MainDlg::ROT )
		return;
	
	mouse_pos = event->pos();
	
	if(event->button() & LeftButton)
	{
		int color = Window->GetColor();
		if( color )
			AnimCube->PipeAdd(Animator::SET_COLOR,
					  1, event->x(), event->y(), color);
	}
}

void GLBox::wheelEvent(QWheelEvent *event)
{
	if( Window->GetColor() )
		AnimCube->PipeAdd(Animator::SET_COLOR,
				  1, event->x(), event->y(),
				  (event->delta() > 0) ? -2 : -1);
}

void GLBox::mouseMoveEvent(QMouseEvent *event)
{
	if( Window->block >= MainDlg::ROT )
		return;
	
	GLfloat dx = (GLfloat)(event->x() - mouse_pos.x()) / width();
	GLfloat dy = (GLfloat)(event->y() - mouse_pos.y()) / height();
  
	if (event->state() & RightButton)
	{
		AnimCube->Cube->angles[1] += dy*180;
		AnimCube->Cube->angles[2] += dx*180;
	}

	mouse_pos = event->pos();
}

void GLBox::timerTick()
{
	updateGL();
}

void GLBox::paintGL()
{
	AnimCube->ProcessPipe();
}

void GLBox::initializeGL()
{
	AnimCube->GLInit();
}

void GLBox::resizeGL( int w, int h )
{
	AnimCube->GLResize(w, h);
}

///------------------------------------------------------

MainDlg::MainDlg(QWidget *parent, const char *name)
	: QDialog(parent, name)
{
	block = ALL;

	toolbox = new QToolBox(this);
	toolbox->setFixedWidth(160);
	
	QSpacerItem *spacer1 = new QSpacerItem(
					0, 15, QSizePolicy::Minimum,
					QSizePolicy::MinimumExpanding );
	
	QSpacerItem *spacer2 = new QSpacerItem(
					0, 15, QSizePolicy::Minimum,
					QSizePolicy::Minimum );

	QWidget *page1 = new QWidget( toolbox );

	QPushButton *load = new QPushButton(
					QPixmap::fromMimeSource("open.png"),
					trUtf8("&Загрузить"), page1 );
	
	QPushButton *save = new QPushButton(
					QPixmap::fromMimeSource("save.png"),
					trUtf8("&Сохранить"), page1 );

	QPushButton *solved = new QPushButton(
					QPixmap::fromMimeSource("home.png"),
					trUtf8("С&бросить"), page1 );

	QPushButton *gray = new QPushButton(
					QPixmap::fromMimeSource("gray.png"),
					trUtf8("&Очистить"), page1 );
	
	QButtonGroup *color = new QButtonGroup( trUtf8("Установить цвет"), page1 );

	radio_color[0] = new QRadioButton( trUtf8("Красный"), color );
	radio_color[1] = new QRadioButton( trUtf8("Зелёный"), color );
	radio_color[2] = new QRadioButton( trUtf8("Синий"), color );
	radio_color[3] = new QRadioButton( trUtf8("Желтый"), color );
	radio_color[4] = new QRadioButton( trUtf8("Оранжевый"), color );
	radio_color[5] = new QRadioButton( trUtf8("Белый"), color );

	QBoxLayout *block1 = new QVBoxLayout( color, 15, 0 );

	block1->addSpacing(10);
	for(int i = 0; i < 6; ++i)
		block1->addWidget(radio_color[i]);

	QSlider *slider = new QSlider( 5, 200, 10, 50, Qt::Horizontal, page1 );
	spin_shuffle = new QSpinBox( 5, 200, 1, page1 );
	spin_shuffle->setValue(50);

	connect(spin_shuffle, SIGNAL(valueChanged(int)),
		slider, SLOT(setValue(int)));
	connect(slider, SIGNAL(valueChanged(int)),
		spin_shuffle, SLOT(setValue(int)));

	block1 = new QHBoxLayout( 0, 6, 6 );
	block1->addWidget(slider);
	block1->addWidget(spin_shuffle);

	QPushButton *shuffle = new QPushButton(
					QPixmap::fromMimeSource("shuffle.png"),
					trUtf8("&Перемешать"), page1 );
	
	connect(solved, SIGNAL(clicked()), this, SLOT(BackToHome()));
	connect(gray,   SIGNAL(clicked()), this, SLOT(BackToGray()));
	connect(save,   SIGNAL(clicked()), this, SLOT(Save()));
	connect(load,   SIGNAL(clicked()), this, SLOT(Open()));
	connect(shuffle,SIGNAL(clicked()), this, SLOT(Shuffle()));
	
	QBoxLayout *block2 = new QVBoxLayout( page1, 0, 6 );
	block2->addItem(spacer1);
	block2->addWidget(load);
	block2->addWidget(save);
	block2->addItem(spacer2);
	block2->addWidget(solved);
	block2->addWidget(gray);
	block2->addItem(spacer2);
	block2->addWidget(color);
	block2->addItem(spacer2);
	block2->addLayout(block1);
	block2->addWidget(shuffle);
	block2->addItem(spacer1);

	QWidget *page2 = new QWidget( toolbox );

	m_num = new QLabel( page2 );
	moves = new QListBox( page2 );
	moves->setHScrollBarMode(QScrollView::AlwaysOff);
	
	patterns = new QComboBox( page2 );
	patterns->insertItem( QPixmap::fromMimeSource("patterns.png"),
			      trUtf8("Узоры") );

	for(int i = 0; i < AnimCube->PatternNum; ++i)
		patterns->insertItem( trUtf8(AnimCube->PatternNames[i]) );

	connect(moves, SIGNAL(highlighted(int)), this, SLOT(ListClicked(int)));
	connect(patterns, SIGNAL(activated(int)), this, SLOT(PatternChosen(int)));

	block2 = new QVBoxLayout( page2, 0, 6 );
	block2->addItem(spacer1);
	block2->addWidget(m_num);
	block2->addWidget(moves, 1);
	block2->addItem(spacer2);
	block2->addWidget(patterns);
	block2->addItem(spacer1);

	QWidget *page3 = new QWidget( toolbox );

	QLabel *exit = new QLabel( trUtf8("Уверены, что хотите выйти?"), page3 );
	exit->setAlignment( QLabel::WordBreak );

	QPushButton *yeah = new QPushButton(
					QPixmap::fromMimeSource("yeah.png"),
					trUtf8("&Да"), page3 );
	
	QPushButton *yep = new QPushButton(
					QPixmap::fromMimeSource("save.png"),
					trUtf8("Да, &сохранив"), page3 );

	QPushButton *nope = new QPushButton(
					QPixmap::fromMimeSource("stop.png"),
					trUtf8("&Нет"), page3 );

	connect(yeah, SIGNAL(clicked()), this, SLOT(reject()));
	connect(yep,  SIGNAL(clicked()), this, SLOT(SaveExit()));
	connect(nope, SIGNAL(clicked()), this, SLOT(ToPage1()));

	block2 = new QVBoxLayout( page3, 0, 6 );
	block2->addItem(spacer1);
	block2->addWidget(exit);
	block2->addItem(spacer2);
	block2->addWidget(yeah);
	block2->addWidget(yep);
	block2->addItem(spacer2);
	block2->addWidget(nope);
	block2->addItem(spacer1);
	
	QFrame *frame = new QFrame( this );
	frame->setFrameStyle( QFrame::Raised | QFrame::GroupBoxPanel );

	GLBox *gl = new GLBox( 60, frame );

	block2 = new QHBoxLayout( frame, 1, 1 );
	block2->addWidget( gl, 1 );
	
	progress = new QProgressBar( this );

	QPushButton *stop = new QPushButton(
				   QPixmap::fromMimeSource("stop.png"), 0, this );

	info = new QLabel( trUtf8("<b>Zauberwürfel-1.0.0</b> by mix_mix"), this );
	info->setFixedWidth(250);
	info->setAlignment( Qt::AlignTop );
	
	toolbox->addItem( page1, trUtf8("Настройка") );
	toolbox->addItem( page2, trUtf8("Собрать") );
	toolbox->addItem( page3, trUtf8("Выход") );

	connect(stop ,SIGNAL(clicked()), this, SLOT(Terminate()));
	connect(toolbox, SIGNAL(currentChanged(int)),
		this, SLOT(ResetToDefault(int)));

	block1 = new QVBoxLayout( 0, 10, 10 );
	block1->addWidget(toolbox);
	block1->addSpacing(30);
	
	block2 = new QHBoxLayout( 0, 0, 3 );
	block2->addWidget(progress);
	block2->addWidget(stop);
	block2->addSpacing(15);
	block2->addWidget(info);

	QBoxLayout *block3 = new QVBoxLayout( 0, 5, 5 );
	block3->addWidget(frame);
	block3->addLayout(block2);
	
	QBoxLayout *block4 = new QHBoxLayout( this, 0, 3 );
	block4->addLayout(block1, 0);
	block4->addLayout(block3, 1);


	
}

///----------------------------------------------------

void MainDlg::BackToHome()
{
	if( block < UI )
	{
		AnimCube->Cube->FaceMap.Reset();
		AnimCube->Cube->FaceMap.SaveState();
		AnimCube->Cube->SetColors();
		SetInfo(0);
	}
}

void MainDlg::BackToGray()
{
	if( block < UI )
	{
		AnimCube->Cube->FaceMap.ResetToGray();
		AnimCube->Cube->SetColors();
		SetInfo(0);
	}
}

int MainDlg::GetColor()
{
	int color = 0;

	for(int i = 0; i < 6; ++i)
		if( radio_color[i]->isChecked() )
		{
			color = i + 1;
			break;
		}

	return color;
}

void MainDlg::SetInfo(const char *format, ...)
{
	if( !format )
	{
		info->clear();
		progress->reset();
	}
	else
	{
		va_list args;
		va_start(args, format);
		
		char str[100];
		vsnprintf(str, 100, format, args);
		info->setText( trUtf8(
			 QString(str).replace(
				 QRegExp("_([^_]*)_"), "<b>\\1</b>" ) ));

		va_end(args);
	}
}

void MainDlg::SetMovesNum(int num)
{
	QString str1, str2;
	str1.setNum(num);

	switch(num % 10)
	{
		case 1:
			str2 = "ход";
			break;
			
		case 2: case 3: case 4:
			str2 = "хода";
			break;
			
		case 5: case 6: case 7:
		case 8: case 9: case 0:
			str2 = "ходов";
			break;
	}

	if(num > 10 && num < 20)
		str2 = "ходов";
		
	
	m_num->setText( trUtf8("<b>" + str1 + "</b> " + str2) );
}

void MainDlg::Save()
{
	if( block >= UI )
		return;
	
	info->clear();
	
	QString fileName = QFileDialog::getSaveFileName(".");
	
	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	file.open(IO_WriteOnly);
	QTextStream out(&file);
	
	out << "CUBE:" << AnimCube->Cube->FaceMap.SavedState << endl;
}

void MainDlg::Open()
{
	QRegExp rx("^CUBE:([RGYBOW]{54})$");

	if( block >= UI )
		return;

	QString fileName = QFileDialog::getOpenFileName(".");
	
	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	file.open(IO_ReadOnly);
	
	QTextStream in(&file);
	QString data = in.readLine();

	if(rx.search(data) == -1)
	{
		info->setText( trUtf8("<b>Неверный формат файла!</b>") );
		return;
	}
	
	AnimCube->Cube->FaceMap.StringToMap(data.ascii() + 5);
	AnimCube->Cube->SetColors();
	AnimCube->Cube->FaceMap.SaveState();
	SetInfo( AnimCube->Cube->FaceMap.Validate() );
}

void MainDlg::Shuffle()
{
	if(block < UI)
		AnimCube->Shuffle( spin_shuffle->value() );
}

void MainDlg::Terminate()
{
	AnimCube->StopSolver();
	Window->block = BAN;
	while(AnimCube->Pipe.GetLength()) Application->processEvents();
	Window->block = NONE;
}

void MainDlg::ListAdd(int move)
{
	QString str;
	QString pix;

	switch(FACE(move))
	{
		case 'F': str = "Фронт "; break;
		case 'B': str = "Тыл ";   break;
		case 'U': str = "Верх ";  break;
		case 'D': str = "Низ ";   break;
		case 'L': str = "Лево ";  break;
		case 'R': str = "Право "; break;
	}

	switch(NUM(move))
	{
		case  2:
		case -2:
			str += "180°";
			pix  = "rot_cw2.png";
			break;
		case  1:
			str += "по час.";
			pix  = "rot_cw.png";
			break;
		case -1:
			str += "против час.";
			pix  = "rot_ccw.png";
			break;
	}

	switch(move)
	{
		case 0:
			str = "Изначально";
			pix = "patterns.png";
			break;
	}
	
	moves->insertItem(QPixmap::fromMimeSource(pix), trUtf8(str));
}

void MainDlg::ListNext(int index)
{
	moves->setCurrentItem( index - 1 );
	moves->ensureCurrentVisible();
}

void MainDlg::ListClear()
{
	moves->clear();
}

void MainDlg::ListClicked(int index)
{
	if( block < MOVE )
		AnimCube->PipeAdd(Animator::ADD_MOVES, 15, index, true, false);
}

void MainDlg::ProgressInit(int total)
{
	progress->reset();
	progress->setProgress(0, total);
}

void MainDlg::ProgressStep(int step)
{
	progress->setProgress( progress->progress() + step );
	
	if( progress->progress() == progress->totalSteps() )
		info->clear();
}

void MainDlg::ProgressIncTotal()
{
	progress->setTotalSteps(progress->totalSteps() + 1);
}

void MainDlg::ResetToDefault(int index)
{
	Terminate();
	progress->reset();
	moves->clear();
	info->clear();
	m_num->clear();

	for(int i = 0; i < 6; ++i)
		radio_color[i]->setChecked(false);

	if(index == 1)
		AnimCube->Solve();
}

void MainDlg::PatternChosen(int index)
{
	patterns->setCurrentItem(0);
	
	if(block < MOVE && index)
		AnimCube->DoPattern(index-1);
}

void MainDlg::ToPage1()
{
	toolbox->setCurrentIndex(0);
}

void MainDlg::SaveExit()
{
	Save();
	reject();
}

void MainDlg::reject()
{
	AnimCube->StopSolver();
	AnimCube->Pipe.Block();
	done(Rejected);
}
