 
#ifndef _GUI_QT_H
#define _GUI_QT_H

#include <qapplication.h>
#include <qdialog.h>
#include <qgl.h>

class QRadioButton;
class QSpinBox;
class QListBox;
class QComboBox;
class QProgressBar;
class QToolBox;
class QLabel;
class QTimer;
class QPoint;

class MainDlg : public QDialog
{
	Q_OBJECT

	public:
		enum {NONE, UI, MOVE, BAN, ROT, ALL};
		
		MainDlg(QWidget *parent = 0, const char *name = 0);

		int  GetColor();
		void SetInfo(const char *format, ...);
		void ListAdd(int move);
		void ListNext(int index);
		void ListClear();
		void ProgressInit(int total);
		void ProgressStep(int step);
		void ProgressIncTotal();
		void SetMovesNum(int num);

		int  block;

	public slots:
		void BackToHome();
		void BackToGray();
		void Save();
		void Open();
		void Shuffle();
		void Terminate();
		void ListClicked(int index);
		void ResetToDefault(int index);
		void PatternChosen(int index);
		void ToPage1();
		void SaveExit();

	protected slots:
		void reject();
		
	private:
		QRadioButton *radio_color[6];
		QSpinBox *spin_shuffle;
		QListBox *moves;
		QToolBox *toolbox;
		QComboBox *patterns;
		QProgressBar *progress;
		QLabel *m_num, *info;
		QString file_filter;
};

class GLBox : public QGLWidget
{
	Q_OBJECT

	public:
		GLBox(int fps, QWidget* parent, const char* name = 0);

	public slots:
		void timerTick();

	protected:
		void initializeGL();
		void paintGL();
		void resizeGL( int w, int h );
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent(QWheelEvent *event);

	private:
		QTimer *gl_timer;
		QPoint mouse_pos;
};

#endif // _GUI_QT_H
