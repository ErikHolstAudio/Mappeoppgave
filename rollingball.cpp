#include "rollingball.h"
#include <QtMath>
#include "visualline.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    mPosition.translate(0.0384,0.0384,0.37); // Starter i venstre hjørne
    //mPosition.translate(0.822, 0.008, 0.08); // Starter i høyre hjørne
    //mPosition.translate(0.01, 0, 0.5f);

    mScale.scale(mRadius,mRadius,mRadius);

    //Debug line
    mVelocityLine = new VisualLine(mVelocity, mPosition, gsml::Vector3d(0,0,0));
    mAccelerationLine = new VisualLine(mAcceleration, mPosition, gsml::Vector3d(0,1,0));
    //mVelocityLine->scale(0.1);
    //mAccelerationLine->scale(1);
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(double* dt)
{

    int trianglesBallIsWithin{0};
    std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    //mMatrix = mPosition * mScale; //Remove later


    for (size_t i = 0; i < vertices.size(); i += 3)
    {
        //Finn trekantens vertices v0, v1, v2
        gsml::Vector3d v0 = vertices.at(i).getXYZ();
        gsml::Vector3d v1 = vertices.at(i+1).getXYZ();
        gsml::Vector3d v2 = vertices.at(i+2).getXYZ();
        //Finn ballens posisjon i xy-planet
        gsml::Vector3d barycResult = getPosition().barycentricCoordinates(v0, v1, v2);

        //Sæk etter triangel som ballen er på nå
        // - med barysentriske koordinater

        //Checks if the ball is within the triangle shape (on x and y axis only)
        if (barycResult.x + barycResult.y + barycResult.z >= 0.99999 && barycResult.x + barycResult.y + barycResult.z <= 1.00001)
        {
            //Sjekk om ballen er innenfor på z aksen


            //beregn normal  // Kunne vært lagret i minne, slikt at vi slipper å kalkulere det hver render.
            gsml::Vector3d triangleNormal = findNormal(v0, v1, v2);

            gsml::Vector3d currentPos = getPosition();

            float distance1 = triangleNormal.dotProduct( currentPos - v1 ) - mRadius;
            std::cout << "distance1: " << distance1 << std::endl;

            if(distance1 <= 0.0001) // Ball is within on z axis
            {
                trianglesBallIsWithin++;

                //beregn akselerasjonvektor - ligning (7)
                float cosAlpha = gsml::Vector3d(0,0,1).dotProduct(triangleNormal); // Finner vinkel mellom normal til xy-aksen og bakken sin normal

                gsml::Vector3d N_force = triangleNormal* mGravity.abs().z * mMass * cosAlpha;

                std::cout << "N Force: "<< N_force << std::endl;
                std::cout << "Cos A: "<< cos(cosAlpha) << std::endl;
                //N_force = 0;
                mAcceleration = (N_force+(mGravity*mMass)) / mMass;

                std::cout << "Acceleration: " <<  mAcceleration << std::endl;

                //Oppdatere hastighet og posisjon
                mVelocity = mVelocity + mAcceleration * 1; // Multiplying here to reduce speed when testing
                std::cout <<"dt: " << std::to_string(*dt) << std::endl;

                mPosition.translate(mVelocity.x * *dt * timeSlowDown, mVelocity.y * *dt * timeSlowDown, mVelocity.z * *dt * timeSlowDown);

                //Fix clipping
                gsml::Vector3d p_0 = v1;
                gsml::Vector3d p = getPosition();

                float distance = triangleNormal.dotProduct( p - p_0 );
                std::cout << "distance: " << distance << std::endl;

                //Moves the ball up, so that it snaps to the triangle
                if(sqrt(distance * distance) <= mRadius/* && sqrt(distance * distance) >= 0*/){
                    gsml::Vector3d offsetResult = triangleNormal * (mRadius - distance);
                    mPosition.translate(offsetResult.x, offsetResult.y, offsetResult.z);
                }

                std::cout << "Velocity: " << mVelocity << std::endl;

                // Ball has crossed over to a new triangle
                if(oldTriangleIndex == -1)
                    oldTriangleIndex = i;

                int newTriangleIndex = i;

                if (newTriangleIndex != oldTriangleIndex /*ny indeks != forrige index*/)
                {
                    //gsml::Vector3d distanceTraveled = mVelocity * *dt;

                    // beregn normalen til kollisjonsplanet, se ligning (9)
                    gsml::Vector3d collisionPlaneNormal = (triangleNormal + oldNormal) / (triangleNormal + oldNormal).length();
                    std::cout <<"CollisionPlanenormal " << collisionPlaneNormal << std::endl;
                    // Korrigere posisjon oppover i normalens retning


                    gsml::Vector3d VelocityAfter = mVelocity - collisionPlaneNormal * (mVelocity.dotProduct(collisionPlaneNormal)) * 2;

                    //VelocityAfter = VelocityAfter + gsml::Vector3d(0,0,-3.3);

                    std::cout << "Velocity: " << mVelocity << std::endl;
                    std::cout << "VelocityAfter: " << VelocityAfter << std::endl;
                    mVelocity = VelocityAfter;

                    //gsml::Vector3d m = gsml::Vector3d::cross(vertices[oldTriangleIndex].to3DVec()*vertices[oldTriangleIndex+1].to3DVec(),vertices[oldTriangleIndex].to3DVec()*vertices[oldTriangleIndex+2].to3DVec());
                    //gsml::Vector3d normalvector = {(m+thisNormalVector)/(m+thisNormalVector).length()};
                    // Oppdater hastighetsvektoren, se ligning (8)
                    // Oppdater posisjon i retning den nye hastighetsvektoren
                }
                oldTriangleIndex = newTriangleIndex;
                oldNormal = triangleNormal;
            }
        }
        else {
            // Calculate ball in free fall
            //beregn normal  // Kunne vært lagret i minne, slikt at vi slipper å kalkulere det hver render.
            //beregn akselerasjonvektor - ligning (7)

        }
    }
    // Freefall
    if(trianglesBallIsWithin == 0){
        mAcceleration = mGravity;
        //Oppdatere hastighet og posisjon
        mVelocity = mVelocity + mAcceleration; // Multiplying here to reduce speed when testing
        mPosition.translate(mVelocity.x * *dt * timeSlowDown, mVelocity.y * *dt * timeSlowDown, mVelocity.z * *dt * timeSlowDown);
    }


    mMatrix = mPosition * mScale;
    return;
}

gsml::Vector3d RollingBall::findNormal(gsml::Vector3d v0, gsml::Vector3d v1, gsml::Vector3d v2)
{
    gsml::Vector3d ac = v2 - v0;
    gsml::Vector3d ab = v1 - v0;
    gsml::Vector3d result = ab.cross(ac);

    return result.getNormalized();
}
void RollingBall::setSurface(VisualObject *surface)
{
    triangle_surface = surface;
}

gsml::Vector3d RollingBall::getPosition()
{
    return gsml::Vector3d(mPosition(0,3), mPosition(1,3), mPosition(2,3));
}

void RollingBall::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(gsml::Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(gsml::Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(gsml::Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Debug Line
    mVelocityLine->init(matrixUniform);
    mAccelerationLine->init(matrixUniform);
}

void RollingBall::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());

    mVelocityLine->draw();
    mAccelerationLine->draw();
}
