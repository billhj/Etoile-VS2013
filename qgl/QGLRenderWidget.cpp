#include "QGLRenderWidget.h"
#include <QTimer>
#include "Camera.h"

namespace Etoile
{
	QGLRenderWidget::QGLRenderWidget(QWidget *parent)
		: QGLWidget(parent)
	{
		ui.setupUi(this);
		defaultSetup();
	}

	QGLRenderWidget::~QGLRenderWidget()
	{

	}

	void QGLRenderWidget::defaultSetup(){
		setFocusPolicy(Qt::StrongFocus);


		m_fpsTime.start();
		m_fpsCounter = 0;
		m_f_p_s = 0.0;
		m_fpsString = tr("%1Hz", "Frames per seconds, in Hertz").arg("?");
		m_displayMessage = false;
		connect(&m_messageTimer, SIGNAL(timeout()), SLOT(hideMessage()));
		m_messageTimer.setSingleShot(true);

		m_fullScreen = false;
		setFullScreen(false);

		m_animationTimerId = 0;
		stopAnimation();
		setAnimationPeriod(40); // 25Hz


		setAttribute(Qt::WA_NoSystemBackground);

	}


	void QGLRenderWidget::initializeGL()
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);

		// Default colors
		setForegroundColor(QColor(180, 180, 180));
		setBackgroundColor(QColor(51, 51, 51));

		// Clear the buffer where we're going to draw
		if (format().stereo())
		{
			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calls user defined method. Default emits a signal.
		init();

		// Give time to glInit to finish and then call setFullScreen().
		if (isFullScreen())
			QTimer::singleShot(100, this, SLOT(delayedFullScreen()));
	}

	void QGLRenderWidget::paintGL()
	{
		if (displaysInStereo())
		{
			for (int view = 1; view >= 0; --view)
			{
				// Clears screen, set model view matrix with shifted matrix for ith buffer
				//preDrawStereo(view);
				
				draw();
				postDraw();
			}
		}
		else
		{
			// Clears screen, set model view matrix...
			preDraw();
	
			draw();
			// Add visual hints: axis, camera, grid...
			postDraw();
		}
		Q_EMIT drawFinished(true);
	}


	/*! Sets OpenGL state before draw().

	Default behavior clears screen and sets the projection and modelView matrices:
	\code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera()->loadProjectionMatrix();
	camera()->loadModelViewMatrix();
	\endcode

	Emits the drawNeeded() signal once this is done (see the <a href="../examples/callback.html">callback example</a>). */
	void QGLRenderWidget::preDraw()
	{
		
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// GL_PROJECTION matrix
		loadProjectionMatrix();
		// GL_MODELVIEW matrix
		loadModelViewMatrix();

		Q_EMIT drawNeeded();
	}

	void QGLRenderWidget::loadProjectionMatrix(bool reset)
	{
		static float* projectM = new float[16];
		glMatrixMode(GL_PROJECTION);
		m_camera->computeProjectionMatrix();
		m_camera->getGLProjectionMatrixf(projectM);
		if (reset)
		{
				glLoadIdentity();
		}
		glMultMatrixf(projectM);
	}

	void QGLRenderWidget::loadModelViewMatrix(bool reset)
	{
		static float* modelVM = new float[16];
		glMatrixMode(GL_MODELVIEW);
		m_camera->computeModelViewMatrix();
		m_camera->getGLModelViewMatrixf(modelVM);
		if (reset)
		{
			glLoadIdentity();
		}
		glMultMatrixf(modelVM);
	}


	/*! Called after draw() to draw viewer visual hints.

	Default implementation displays axis, grid, FPS... when the respective flags are sets.

	See the <a href="../examples/multiSelect.html">multiSelect</a> and <a
	href="../examples/contribs.html#thumbnail">thumbnail</a> examples for an overloading illustration.

	The GLContext (color, LIGHTING, BLEND...) is \e not modified by this method, so that in
	draw(), the user can rely on the OpenGL context he defined. Respect this convention (by pushing/popping the
	different attributes) if you overload this method. */
	void QGLRenderWidget::postDraw()
	{
		// Reset model view matrix to world coordinates origin
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		loadModelViewMatrix();
		// TODO restore model loadProjectionMatrixStereo

		// Save OpenGL state
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// Set neutral GL state
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);

		glDisable(GL_TEXTURE_GEN_Q);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);


		glDisable(GL_COLOR_MATERIAL);
		qglColor(foregroundColor());


		// FPS computation
		const unsigned int maxCounter = 20;
		if (++m_fpsCounter == maxCounter)
		{
			m_f_p_s = 1000.0 * maxCounter / m_fpsTime.restart();
			m_fpsString = tr("%1Hz", "Frames per seconds, in Hertz").arg(m_f_p_s, 0, 'f', ((m_f_p_s < 10.0) ? 1 : 0));
			m_fpsCounter = 0;
		}

		// Restore foregroundColor
		float color[4];
		color[0] = foregroundColor().red() / 255.0f;
		color[1] = foregroundColor().green() / 255.0f;
		color[2] = foregroundColor().blue() / 255.0f;
		color[3] = 1.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		if (FPSIsDisplayed()) displayFPS();
		if (m_displayMessage) drawText(10, height() - 10, m_message);

		// Restore GL state
		glPopAttrib();
		glPopMatrix();
	}


	/*! Draws a simplified version of the scene to guarantee interactive camera displacements.

	This method is called instead of draw() when the qglviewer::Camera::frame() is
	qglviewer::ManipulatedCameraFrame::isManipulated(). Default implementation simply calls draw().

	Overload this method if your scene is too complex to allow for interactive camera manipulation. See
	the <a href="../examples/fastDraw.html">fastDraw example</a> for an illustration. */
	void QGLRenderWidget::fastDraw()
	{
		draw();
	}


	void QGLRenderWidget::resizeGL(int width, int height)
	{

	}


	void QGLRenderWidget::displayFPS()
	{
		drawText(10, int(1.5*((QApplication::font().pixelSize() > 0) ? QApplication::font().pixelSize() : QApplication::font().pointSize())), m_fpsString);
	}

	void QGLRenderWidget::drawText(int x, int y, const QString& text, const QFont& fnt)
	{
		if (!m_textIsEnabled)
			return;
		renderText(x, y, text, fnt);
	}

	void QGLRenderWidget::setFullScreen(bool fullScreen)
	{
		if (m_fullScreen == fullScreen) return;

		m_fullScreen = fullScreen;

		QWidget* tlw = topLevelWidget();

		/*if (isFullScreen())
		{
		prevPos_ = topLevelWidget()->pos();
		tlw->showFullScreen();
		tlw->move(0, 0);
		}
		else
		{
		tlw->showNormal();
		tlw->move(prevPos_);
		}*/
	}

	/*! Starts the animation loop. See animationIsStarted(). */
	void QGLRenderWidget::startAnimation()
	{
		m_animationTimerId = startTimer(animationPeriod());
		m_animationStarted = true;
	}

	/*! Stops animation. See animationIsStarted(). */
	void QGLRenderWidget::stopAnimation()
	{
		m_animationStarted = false;
		if (m_animationTimerId != 0)
			killTimer(m_animationTimerId);
	}
}