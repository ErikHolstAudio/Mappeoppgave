#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <string>
#include "renderwindow.h"

#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "xyz.h"
#include "tetrahedron.h"
#include "cube.h"
#include "trianglesurface.h"
#include "disc.h"
#include "route.h"
#include "npc.h"
#include "beziercurve.h"
#include "token.h"
#include "octahedronball.h"
#include "door.h"
#include "scene1.h"
#include "scene2.h"
#include "scene3.h"
#include "examscene.h"
#include "physicsscene.h"
#include "texture.h"
#include "shader.h"
#include "quad.h"
#include "triangle.h"
#include "shaderhandler.h"
#include "light.h"
#include "camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "rollingball.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //This is the matrix used to transform (rotate) the triangle
    //You could do without, but then you have to simplify the shader and shader setup
    //mMVPmatrix = new QMatrix4x4{};
    //mMVPmatrix->setToIdentity();    //1, 1, 1, 1 in the diagonal of the matrix
    mPmatrix = new QMatrix4x4{};
    mPmatrix->setToIdentity();
    mVmatrix = new QMatrix4x4{};
    mVmatrix->setToIdentity();

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    } 

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();


    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
	glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //set up alpha blending for textures
    glEnable(GL_BLEND);// you enable blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mShaderHandler = new ShaderHandler();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    mScenes.push_back(new Scene1(mScenes, mShaderHandler, *this, 20.f));
    mScenes.push_back(new Scene2(mScenes, mShaderHandler, *this, 15.f));
    mScenes.push_back(new Scene3(mScenes, mShaderHandler, *this));

    //mScenes.push_back(new examscene(mScenes, mShaderHandler, *this));
    mScenes.push_back(new PhysicsScene(mScenes, mShaderHandler, *this));

    for (auto it = mScenes.begin(); it != mScenes.end(); it++)
        (*it)->updateScenes(mScenes);

    activeScene = mScenes[3];
    activeScene->activateScene();

    player = activeScene->getPlayer();
    mCamera = activeScene->getCamera();


    /*
    // ********************** Compile shaders: **********************
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram[0] = new Shader("../Mappeoppgave/plainshader.vert", "../Mappeoppgave/plainshader.frag");
    mLogger->logText("Plain shader program id: " + std::to_string(mShaderProgram[0]->getProgram()) );

    mShaderProgram[1]= new Shader("../Mappeoppgave/textureshader.vert", "../Mappeoppgave/textureshader.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[1]->getProgram()) );

    mShaderProgram[2] = new Shader("../Mappeoppgave/phongshader.vert", "../Mappeoppgave/phongshader.frag");
    mLogger->logText("Texture shader program id: " + std::to_string(mShaderProgram[2]->getProgram()));

    setupPlainShader(0);
    setupTextureShader(1);
    setupPhongShader(2);
    */

    //********************** Texture stuff: **********************
    //Returns a pointer to the Texture class. This reads and sets up the texture for OpenGL
    //and returns the Texture ID that OpenGL uses from Texture::id()
//    mTexture[0] = new Texture();
//    mTexture[1] = new Texture("../Mappeoppgave/Assets/hund.bmp");

    //Set the textures loaded to a texture unit (also called a texture slot)
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, mTexture[0]->id());
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, mTexture[1]->id());

    //********************** Making the object to be drawn **********************

    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader

    // Flere matriser her! Skal legges inn i kameraklasse
    //mMatrixUniform = glGetUniformLocation( mShaderProgram[0]->getProgram(), "matrix" );
    //mPmatrixUniform = glGetUniformLocation( mShaderProgram[0]->getProgram(), "pmatrix" );
    //mVmatrixUniform = glGetUniformLocation( mShaderProgram[0]->getProgram(), "vmatrix" );

    mCamera->init(mShaderHandler->pMatrixUniform0, mShaderHandler->vMatrixUniform0);


    for (auto it = mScenes.begin(); it != mScenes.end(); it++)
        (*it)->init();

    glBindVertexArray(0);       //unbinds any VertexArray - good practice

}


// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //what shader to use
    glUseProgram(mShaderHandler->mShaderProgram[0]->getProgram() );

    for (auto it = mScenes.begin(); it != mScenes.end(); it++)
        if ((*it)->checkActivated())
        {
            activeScene = (*it);
            break;
        }
            
    player = activeScene->getPlayer();
    mCamera = activeScene->getCamera();

    activeScene->draw();

    glUseProgram(0); // Makes a NVIDIA shader warning go away

    //Calculate framerate before
    // checkForGLerrors() because that call takes a long time
    // and before swapBuffers(), else it will show the vsync time
    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    //checkForGLerrors();

    //Qt require us to call this swapBuffers() -function.
    // swapInterval is 1 by default which means that swapBuffers() will (hopefully) block
    // and wait for vsync.

    mContext->swapBuffers(this);
}

void RenderWindow::showEquidistance(bool checked)
{
    for(int i{};i<mScenes.size(); i++){
        if(activeScene){
            dynamic_cast<PhysicsScene*>(activeScene)->showEquidistance(checked);
        }
    }
}

void RenderWindow::showSurface(bool checked)
{
    for (int i{};i<mScenes.size() ;i++ ) {
        if(activeScene){
            dynamic_cast<PhysicsScene*>(activeScene)->showSurface(checked);
        }
    }
}

void RenderWindow::showPoint(bool checked)
{
    for (int i{};i<mScenes.size() ;i++ ) {
        if(activeScene){
            dynamic_cast<PhysicsScene*>(activeScene)->showPointCloud(checked);
        }
    }
}

void RenderWindow::spawnBalls(int value)
{
    for(int i{}; i < mScenes.size(); i++){
        if(activeScene){
            dynamic_cast<PhysicsScene*>(activeScene)->spawnBalls(value);
        }
    }
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

void RenderWindow::drawAABB(glm::vec3 position, glm::vec3 extent)
{
    static uint32_t VAO{};
    if (VAO == 0)
    {
        std::vector<gsml::Vertex> verts;
        float size{ 1.f };
        verts.push_back(gsml::Vertex{ -size,  size, -size, 1,1,1 });
        verts.push_back(gsml::Vertex{  size,  size, -size, 1,1,1 });
        verts.push_back(gsml::Vertex{  size,  size,  size, 1,1,1 });
        verts.push_back(gsml::Vertex{ -size,  size,  size, 1,1,1 });
        verts.push_back(gsml::Vertex{ -size, -size, -size, 1,1,1 });
        verts.push_back(gsml::Vertex{  size, -size, -size, 1,1,1 });
        verts.push_back(gsml::Vertex{  size, -size,  size, 1,1,1 });
        verts.push_back(gsml::Vertex{ -size, -size,  size, 1,1,1 });
        std::vector<GLuint> indices{ 0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7 };

        uint32_t VBO{};
        uint32_t EAB{};

        initializeOpenGLFunctions();
        //Vertex Array Object - VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //Vertex Buffer Object to hold vertices - VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(gsml::Vertex), verts.data(), GL_STATIC_DRAW);

        // 1rst attribute buffer : vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // 2nd attribute buffer : colors
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // 3rd attribute buffer : uvs
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(gsml::Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        //Second buffer - holds the indices (Element Array Buffer - EAB):
        glGenBuffers(1, &EAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    glm::mat4 matrix(1.f);
    matrix[3].x = position.x;
    matrix[3].y = position.y;
    matrix[3].z = position.z;

    matrix[0].x = extent.x;
    matrix[1].y = extent.y;
    matrix[2].z = extent.z;

    glUseProgram(mShaderHandler->mShaderProgram[0]->getProgram());

    glBindVertexArray(VAO);
    glUniformMatrix4fv(mShaderHandler->mMatrixUniform0,          //the location of the matrix in the shader
        1,                       //count
        GL_FALSE,                //transpose the matrix before sending it?
        glm::value_ptr(matrix));    //the data of the matrix

    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (mCamera->bFreeCamera)
        mCamera->mouseMoveEvent(event);
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        event->ignore();

    else
    {
        if (mCamera->bFreeCamera)
            mCamera->keyPressEvent(event);
        else
			player->keyPressEvent(event);

        activeScene->keyPressEvent(event);


	    if (event->key() == Qt::Key_Escape)
	        mMainWindow->close();       //Shuts down the whole program

        if (event->key() == Qt::Key_Space)
            activeScene->triggerCheck();

        if (event->key() == Qt::Key_F)
            wireframeToggle();

        if (event->key() == Qt::Key_C)
            cullToggle();

        if(event->key()==Qt::Key_N)
            bDrawEquidistance = !bDrawEquidistance;

        if(event->key()==Qt::Key_M)
            bDrawPointCloud = !bDrawPointCloud;
        if (event->key() == Qt::Key_B)
        {
            if (!bDrawCollision)
                bDrawCollision = true;
            else
                bDrawCollision = false;
        }

        if (event->key() == Qt::Key_R)
        {
	        if (!activeScene->bDrawQuads)
                activeScene->bDrawQuads = true;
            else
                activeScene->bDrawQuads = false;
        }
        if (event->key() == Qt::Key_4)
        {
            activeScene->deactivateScene();
            activeScene = mScenes[3];
            activeScene->activateScene();
        }
    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        event->ignore();

    else
    {
        if (mCamera->bFreeCamera)
            mCamera->keyReleaseEvent(event);
        else
	        player->keyReleaseEvent(event);
        activeScene->keyReleaseEvent(event);
    }
}

void RenderWindow::wireframeToggle()
{
    static bool toggle = true;
    if (toggle)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        toggle = !toggle;
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        toggle = !toggle;
    }
}

void RenderWindow::cullToggle()
{
    static bool toggle = true;
    if (toggle)
    {
        glDisable(GL_CULL_FACE);
        toggle = !toggle;
    }
    else
    {
        glEnable(GL_CULL_FACE);
        toggle = !toggle;
    }
}

//Oppgave 5
void RenderWindow::debugCameraToggle()
{
    if (mCamera->bFreeCamera == false)
        mCamera->bFreeCamera = true;
    else
        mCamera->bFreeCamera = false;
}
//Oppgave 6 (trykk på debug mode)
void RenderWindow::debugModeToggle()
{
    if (bDebugMode == false)
    {
        debugCameraToggle();
        bDebugMode = true;
    }
    else
    {
        debugCameraToggle();
        bDebugMode = false;
    }
}

void RenderWindow::setNum(int value)
{
    RollingBall* RB{nullptr};
    for (int i{}; i<=value ;i++ ) {
        //RB=new RollingBall(*this, mShaderHandler->mShaderProgram[1], 0.5f,QVector3D(0.f,0.75f,0.75f))
    }
}
