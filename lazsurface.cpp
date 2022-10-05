#include "lazsurface.h"
#include <iomanip> //for std::setprecision
#include "equidistance.h"
#include "renderwindow.h"

LAZSurface::LAZSurface(const std::string txtFileName, const QVector2D gridSize, Scene& scene, Shader* shaderProgram, const QVector3D offset, const float scale) :
    mScale(scale), mOffset(offset), mGridSizeX(gridSize.x()), mGridSizeY(gridSize.y()), VisualObject(scene, shaderProgram)
{
    construct(txtFileName);

    mMatrix.setToIdentity();
}

void LAZSurface::construct(std::string txtFileName)
{
    // Read file
    readFile(txtFileName);
}

bool compare(QVector3D i1,QVector3D i2)
{
    if(i1.x() == i2.x()){
        if(i1.y() < i2.y())
            return 1;
    }

    return (i1.x() < i2.x());
}

void LAZSurface::readFile(std::string txtFileName)
{
    std::ifstream inn;

    inn.open(txtFileName.c_str());

    if(inn.is_open()){

        //Get the number of points from the txt file (first line)
        std::string amountStr;
        int amount;
        inn >> amount;
        std::cout << amount << std::endl;

        std::vector<QVector3D> points;

        //Get point data and set it into an array
        for (int i = 0; i < amount; i++) //std::getline(inn, str, ' ')
        {
            float x,y,z;
            inn >> x >> y >> z;
            points.push_back(QVector3D(x + mOffset.x(), y + mOffset.y(), z + mOffset.z()));

            //Initializes x and y min/max
            if(i == 0){
                xMin = x + mOffset.x();
                xMax = xMin;
                yMin = y + mOffset.y();
                yMax = yMin;
                zMin = z + mOffset.z();
                zMax = zMin;
            }

            // Find X and Y max/min
            if(x + mOffset.x() > xMax)
                xMax = x + mOffset.x();
            else if(x + mOffset.x() < xMin)
                xMin = x + mOffset.x();
            if(y + mOffset.y() > yMax)
                yMax = y + mOffset.y();
            else if(y + mOffset.y() < yMin)
                yMin = y + mOffset.y();
            if(z + mOffset.z() > zMax)
                zMax = z + mOffset.z();
            else if(z + mOffset.z() < zMin)
                zMin = z + mOffset.z();
        }
        std::cout << std::fixed << std::setprecision(7)<< "LAZ file; xMin: " << xMin << ", xMax: " << xMax << ", yMin: " << yMin << ", yMax: " << yMax << ", zMin: " << zMin << ", zMax: " << zMax << std::endl;

        // Sort - Not needed
        //std::sort(points.begin(), points.end(), compare);

        // Fill 'mHeightsInSquare' with empty data. This is used to keep track of the height positions of all the points
        mHeightInArea.resize(mGridSizeX);
        for(int i = 0; i < mGridSizeX; i++){
            mHeightInArea.at(i).resize(mGridSizeY);
        }

        // Find increment
        float xInc = (xMax - xMin)/(mGridSizeX); // 'inc' stands for increment
        float yInc = (yMax - yMin)/(mGridSizeY);

        // Find z height for all verticies
        for(int i = 0; i < points.size(); i++){
            QVector3D point = points.at(i);
            // X & Y area index.
            // It is ment to be overwritten, but in case of decimal error; then it is highly likely that it is supposed to be the largest index in the vector.
            int xAreaI{mGridSizeX-1}, yAreaI{mGridSizeY-1};

            // Find out which of the divided zones the height coordinate should belong in
            //Get x area index
            for(int j = 0; j < mGridSizeX; j++){ // could be faster by using quick sort
                if(xMin + (xInc * j) <= point.x() && point.x() <= xMin + xInc + (xInc * j)){
                    xAreaI = j;
                    break;
                }
            }

            //Get y area index
            for(int j = 0; j < mGridSizeY; j++){ // could be faster by using quick sort
                if(yMin + (yInc * j) <= point.y() && point.y() <= yMin + yInc + (yInc * j)){
                    yAreaI = j;
                    break;
                }
            }
            // Insert the points height into mHeightInArea vector, while also calulating the average height out of all the points (in that specific area)
            addToAverageHeight(xAreaI, yAreaI, point.z());
        }

        // Create the surface
        for(int b = 0; b < mGridSizeY; b++) {
            for(int a = 0; a < mGridSizeX; a++) {
                //Height
                float z1 = getAverageHeight(a,b);

                //Get float position from index;
                float x = xMin + xInc * a;
                float y = yMin + yInc * b;

                //Color - Checker style
                //                QVector3D color;
                //                if((a % 2 + b % 2) % 2)
                //                    color = QVector3D(1,1,1);
                //                else
                //                    color = QVector3D(0,0,0);
                float zDistance = (zMax - zMin) ;

                //Vertex
                mVertices.push_back(gsml::Vertex{x,y,z1, 0,z1/zDistance,0});


            }
        }
        for(int b = 0; b < mGridSizeY - 1; b++) {
            for(int a = 0; a < mGridSizeX -1; a++) {
                //Indicies for creating a square
                mIndices.push_back(a + b*mGridSizeX);
                mIndices.push_back(a+1 + b*mGridSizeX);
                mIndices.push_back(a+1 + (b+1)*mGridSizeX);

                mIndices.push_back(a + b*mGridSizeX);
                mIndices.push_back(a+1 + (b+1)*mGridSizeX);
                mIndices.push_back(a + (b+1)*mGridSizeX);
                //std::cout << "a: " << a << ", b: " << b << std::endl;
            }
        }
        std::cout << "vertices: " << mVertices.size() << std::endl;
        std::cout << "indices: " << mIndices.size() << std::endl;
    }
    else{
        std::cout << "Error, txt file '" << txtFileName << "' was not found, LAZ surface.\n";
        return;
    }

    //Clear height points to free up some memory
    mHeightInArea.clear();

    inn.close();

    // Read file and store data.
    // Sort data while storing. X first, then check on Y axis
}

void LAZSurface::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( gsml::Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(gsml::Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( gsml::Vertex ),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);


    // 3rd attribute buffer : uvs
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void LAZSurface::draw()
{
    drawEquidistanceLines = RenderWindow::bDrawEquidistance; //mMaterial->UpdateUniforms(&mMatrix);

    if(mEquiLines && drawEquidistanceLines)
        mEquiLines->draw();

    //glBindVertexArray( mVAO );
    //glDrawArrays(GL_TRIANGLES , 0, mVertices.size());

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}



void LAZSurface::addToAverageHeight(int xIndex, int yIndex, float height)
{
    mHeightInArea.at(xIndex).at(yIndex).height += height;
    mHeightInArea.at(xIndex).at(yIndex).iteration++;
}

float LAZSurface::getAverageHeight(int x, int y)
{
    // If no points exists within the area, return 0 as height for the area
    if(mHeightInArea.at(x).at(y).iteration == 0) return 0;

    // If one or more points exists within the area, return average height in area
    return (mHeightInArea.at(x).at(y).height / mHeightInArea.at(x).at(y).iteration);
}

Equidistance* LAZSurface::constructEquidistance()
{
    if(mEquiLines)
    {
        std::cout << "Error! Equidistance has already ben constructed! A second attemt to construct it has been made" << std::endl;
        return mEquiLines;
    }

    mEquiLines = new Equidistance(mScene, mShaderProgram);
    drawEquidistanceLines = true;
    // Do a check for each triangle (double for loop)
    for(int i = 0; i < mIndices.size(); i+=3){
        gsml::Vertex* abc[3]{nullptr};
        abc[0] = &mVertices.at(mIndices.at(i));
        abc[1] = &mVertices.at(mIndices.at(i+1));
        abc[2] = &mVertices.at(mIndices.at(i+2));

        gsml::Vertex* temp{nullptr};

        // Sort the three vertices by height (Low to high)
        if(abc[0]->getXYZ().z > abc[1]->getXYZ().z){
            temp = abc[0];
            abc[0] = abc[1];
            abc[1] = temp;
        }
        if(abc[1]->getXYZ().z > abc[2]->getXYZ().z){
            temp = abc[1];
            abc[1] = abc[2];
            abc[2] = temp;
        }
        if(abc[0]->getXYZ().z > abc[1]->getXYZ().z){
            temp = abc[0];
            abc[0] = abc[1];
            abc[1] = temp;
        }

        float minHeight = abc[0]->getXYZ().z;
        float midHeight = abc[1]->getXYZ().z;
        float maxHeight = abc[2]->getXYZ().z;

        //Check if triangle lays flat on the equidistance, if so don't add lines and return
        if(fmod(minHeight, mEquidistance) == 0 && maxHeight == minHeight ) return mEquiLines; // Could potentially alter the color the entire square before returning


        // Checks if there should be a line drawn at the bottom of the triangle
        if(midHeight == minHeight && midHeight != maxHeight && fmod(minHeight, mEquidistance) == 0)
            mEquiLines->insertLine(abc[0]->getXYZ(), abc[1]->getXYZ());

        //Checks if the triangle has an equidistance line passing through it
        if(maxHeight >= minHeight + mEquidistance - fmod(minHeight, mEquidistance))
        {
            //There is at least one line passing through the triangle, so the first line gets created
            QVector3D p0, p1, direction;
            float checkHeight = minHeight + mEquidistance - fmod(minHeight, mEquidistance);
            // Every line crossing the triangle HAS to at least have one point on the line between the lowest and the highest vertices
            //First point
            direction = abc[2]->getXYZ() - abc[0]->getXYZ();
            direction.normalize();
            float multiplier = (checkHeight - minHeight)/direction.z();
            p0 = abc[0]->getXYZ() + (direction * multiplier);
            // Second point
            if(checkHeight <= midHeight) // Second point is on line AB
            {
                direction = abc[1]->getXYZ() - abc[0]->getXYZ();
                direction.normalize();
                float multiplier = (checkHeight - minHeight)/direction.z();
                p1 = abc[0]->getXYZ() + (direction * multiplier);
            }
            else // Second point is on line BC
            {
                direction = abc[2]->getXYZ() - abc[1]->getXYZ();
                direction.normalize();
                float multiplier = (checkHeight - midHeight)/direction.z();
                p1 = abc[1]->getXYZ() + (direction * multiplier);
            }
            mEquiLines->insertLine(p0, p1);

            // Then continue checking if there are more equidistance lines on the same triangle
            // Checks if there is a line passing through
            while(checkHeight + mEquidistance <= maxHeight){
                checkHeight += mEquidistance;
                // The line should be drawn on the top of the triangle
                if(maxHeight == minHeight && checkHeight + mEquidistance == maxHeight){
                    mEquiLines->insertLine(abc[1]->getXYZ(), abc[2]->getXYZ());
                }
                // The line should be going through the triangle
                else {
                    //First point
                    direction = abc[2]->getXYZ() - abc[0]->getXYZ();
                    direction.normalize();
                    float multiplier = (checkHeight - minHeight)/direction.z();
                    p0 = abc[0]->getXYZ() + (direction * multiplier);

                    // Second point
                    if(checkHeight <= midHeight) // Second point is on line AB
                    {
                        direction = abc[1]->getXYZ() - abc[0]->getXYZ();
                        direction.normalize();
                        float multiplier = (checkHeight - minHeight)/direction.z();
                        p1 = abc[0]->getXYZ() + (direction * multiplier);
                    }
                    else // Second point is on line BC
                    {
                        direction = abc[2]->getXYZ() - abc[1]->getXYZ();
                        direction.normalize();
                        float multiplier = (checkHeight - midHeight)/direction.z();
                        p1 = abc[1]->getXYZ() + (direction * multiplier);
                    }
                    mEquiLines->insertLine(p0, p1);
                }
            }
        }
    }

    mEquiLines->init();
    return mEquiLines;
}

std::vector<QVector3D> LAZSurface::getTriangleVertices(QVector3D pos)
{
    // Checks if the triangle is within the surface area
    if(xMin <= pos.x() && pos.x() <= xMax && yMin <= pos.y() && pos.y() <= yMax){
        // Finding using quick sort instead of linear search would more efficient

        // Find increment
        float xInc = (xMax - xMin)/(mGridSizeX); // 'inc' stands for increment
        float yInc = (yMax - yMin)/(mGridSizeY);

        // Find which square are the ball is within
        for(int y = 0; y < mGridSizeY; y++){
            for(int x = 0; x < mGridSizeX; x++){
                if(xMin + x * xInc <= pos.x() && xMin + (x+1) * xInc > pos.x() &&
                    yMin + y * yInc <= pos.y() && yMin + (y+1) * yInc > pos.y())
                {
                    // Square found
                    // Use BarycentricCoordinates to find out which square
                    int v0 = x + y*mGridSizeX;
                    int v1 = x+1 + y*mGridSizeX;
                    int v2 = x+1 + (y+1)*mGridSizeX;
                    //int v3 = x + (y+1)*mGridSizeX; // Unused when commented out

                    QVector3D barycResult;
                    barycResult = calcBarycentric(pos.toVector2D(), mVertices[v0].getXYZ().toVector2D(), mVertices[v1].getXYZ().toVector2D(), mVertices[v2].getXYZ().toVector2D());

                    //Checks the first triangle if the ball is within on the x and y axis
                    if (barycResult.x() + barycResult.y() + barycResult.z() >= 0.99999 && barycResult.x() + barycResult.y() + barycResult.z() <= 1.00001)
                    {
                        std::vector<QVector3D> result;
                        result.push_back(mVertices[v0].getXYZ());
                        result.push_back(mVertices[v1].getXYZ());
                        result.push_back(mVertices[v2].getXYZ());
                        return result;
                    }

                    //Checks the second triangle
                    barycResult = calcBarycentric(pos.toVector2D(), mVertices[v0].getXYZ().toVector2D(), mVertices[v1].getXYZ().toVector2D(), mVertices[v2].getXYZ().toVector2D());
                    if(barycResult.x() + barycResult.y() + barycResult.z() >= 0.99999 && barycResult.x() + barycResult.y() + barycResult.z() <= 1.00001)
                    {
                        std::vector<QVector3D> result;
                        result.push_back(mVertices[v0].getXYZ());
                        result.push_back(mVertices[v1].getXYZ());
                        result.push_back(mVertices[v2].getXYZ());
                        return result;
                    }
                    else {std::cout << "Error! Something went wrong in LAZSurface, when calculating Barycentric coordinates, in getTriangleVertices"; }

                }
            }
        }
    }

    std::cout << "Error! No triangle found, when searching in LAZSurface" << std::endl;
    std::vector<QVector3D> result;
    //Returns empty if no triangle was found
    return result;
}

QVector3D LAZSurface::calcBarycentric(QVector2D point, QVector2D p1, QVector2D p2, QVector2D p3) {
    //Vab & Vac
    QVector2D Va = p2 - p1;
    QVector2D Vb = p3 - p1;
    QVector3D n = QVector3D::crossProduct(QVector3D(Va.x(), Va.y(), 0), QVector3D(Vb.x(), Vb.y(), 0));
    float areal_123 = n.length();
    QVector3D baryc;

    // u
    QVector2D p = p2 - point;
    QVector2D q = p3 - point;
    n = QVector3D::crossProduct(QVector3D(p.x(), p.y(), 0), QVector3D(q.x(), q.y(), 0));
    baryc.setX( n.z() / areal_123 );

    // v
    p = p3 - point;
    q = p1 - point;
    n = QVector3D::crossProduct(QVector3D(p.x(), p.y(), 0), QVector3D(q.x(), q.y(), 0));

    baryc.setY( n.z() / areal_123 );

    // w
    p = p1 - point;
    q = p2 - point;
    n = QVector3D::crossProduct(QVector3D(p.x(), p.y(), 0), QVector3D(q.x(), q.y(), 0));
    baryc.setZ( n.z() / areal_123 );

    return baryc;
}
