#ifndef QGLRENDERWIDGET_H
#define QGLRENDERWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QTimer>
#include "ui_QGLRenderWidget.h"


namespace Etoile
{
	class Camera;
	class QGLRenderWidget : public QGLWidget
	{
		Q_OBJECT

	public:
		QGLRenderWidget(QWidget *parent = 0);
		~QGLRenderWidget();
		void defaultSetup();
		bool displaysInStereo() const { return m_stereo; }
		QColor backgroundColor() const { return m_backgroundColor; }
		QColor foregroundColor() const { return m_foregroundColor; }
		bool FPSIsDisplayed() const { return m_FPSIsDisplayed; }
		void drawText(int x, int y, const QString& text, const QFont& fnt = QFont());
		qreal currentFPS() { return m_f_p_s; }
		bool isFullScreen() const { return m_fullScreen; }
		bool animationIsStarted() const { return m_animationStarted; }
		int animationPeriod() const { return m_animationPeriod; }
		Camera* camera(){ return m_camera; }


		void loadProjectionMatrix(bool reset = false);
		void loadModelViewMatrix(bool reset = false);
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);

		virtual void init() = 0;
		virtual void draw() = 0;
		virtual void preDraw();
		virtual void fastDraw();
		virtual void postDraw();

		public Q_SLOTS:
		/*! Sets the backgroundColor() of the viewer and calls \c qglClearColor(). See also
		setForegroundColor(). */
		void setBackgroundColor(const QColor& color) { m_backgroundColor = color; qglClearColor(color); }
		/*! Sets the foregroundColor() of the viewer, used to draw visual hints. See also setBackgroundColor(). */
		void setForegroundColor(const QColor& color) { m_foregroundColor = color; }

		void displayFPS();
		void setFullScreen(bool fullScreen = true);
		void setStereoDisplay(bool stereo = true);
		void setAnimationPeriod(int period) { m_animationPeriod = period; }
		virtual void startAnimation();
		virtual void stopAnimation();
	public:
	Q_SIGNALS :
		/*! Signal emitted by the default init() method.
		Connect this signal to the methods that need to be called to initialize your viewer or overload init(). */
		void viewerInitialized();
			  /*! Signal emitted by the default draw() method.
			  Connect this signal to your main drawing method or overload draw(). See the <a
			  href="../examples/callback.html">callback example</a> for an illustration. */
			  void drawNeeded();
			  /*! Signal emitted at the end of the QGLViewer::paintGL() method, when frame is drawn.
			  connect(viewer, SIGNAL(drawFinished(bool)), SLOT(saveSnapshot(bool)));*/
			  void drawFinished(bool automatic);

	private:
		Ui::QGLRenderWidget ui;
		QColor m_backgroundColor, m_foregroundColor;
		bool m_stereo;
		bool m_fullScreen;
		bool m_FPSIsDisplayed;
		bool m_textIsEnabled;	// drawText() actually draws text or not

		QTime m_fpsTime;
		unsigned int m_fpsCounter;
		QString m_fpsString;
		qreal m_f_p_s;

		QString m_message;
		bool m_displayMessage;
		QTimer m_messageTimer;

		bool m_animationStarted; // animation mode started
		int m_animationPeriod;
		int m_animationTimerId;
		Camera* m_camera;
	};
}
#endif // QGLRENDERWIDGET_H
