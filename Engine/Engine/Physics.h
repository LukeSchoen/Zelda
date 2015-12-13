#ifndef Physics_h__
#define Physics_h__

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "OBJ Loader.h"

#pragma region structs

struct Collision
{
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 Elipsoid;
  glm::vec3 lastSafePosition;
  glm::vec3 ellipsoidIntersectionPoint;
  glm::vec3 PolygonIntersectionPoint;
  bool collisionFound = false;
  bool isStuck = false;
  double collisionTvalue;
};

#pragma endregion

namespace // internal physics
{
#pragma region statics

  static const float MovementEpsilon = 0.05f; // The smallest movement worth doing
  static Collision WorldCollision;

#pragma endregion

  // Projection

  glm::vec3 ProjectPointOntoLine(glm::vec3& lineStart, glm::vec3& lineEnd, glm::vec3& point)
  {
    glm::vec3 pointToLineStart = point - lineStart;
    glm::vec3 line = lineEnd - lineStart;
    double d = glm::length(line);
    line = glm::normalize(line);
    double t = dot(line, pointToLineStart);
    if (t < 0.0f) return (lineStart); // T is before line start
    if (t > d) return (lineEnd); // T is after line end
    line.x = line.x * t;
    line.y = line.y * t;
    line.z = line.z * t;
    return (lineStart + line); // T is within the line segment
  }

  double ProjectRayOntoPlane(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 planeOrigin, glm::vec3 planeNormal)
  {
    double projection = -(dot(planeNormal, planeOrigin));
    double numerator = dot(planeNormal, rayOrigin) + projection;
    double demoniator = dot(planeNormal, rayDirection);
    if (demoniator == 0)  // plane is anisotropic with ray!
      return -1.0f;
    return -(numerator / demoniator);
  }

  glm::vec3 ProjectPointOntoTriangle(glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3, glm::vec3 point)
  {
    glm::vec3 lineToVert12 = ProjectPointOntoLine(vert1, vert2, point);
    glm::vec3 lineToVert23 = ProjectPointOntoLine(vert2, vert3, point);
    glm::vec3 lineToVert31 = ProjectPointOntoLine(vert3, vert1, point);
    double distToVert12 = glm::length(point - lineToVert12);
    double distToVert23 = glm::length(point - lineToVert23);
    double distToVert31 = glm::length(point - lineToVert31);
    //Find the closest vert
    double closest = distToVert12;   // this is probably causing a bug!
    glm::vec3 result = lineToVert12; // getting closest vert is not good enough
    if (distToVert23 < closest)
    {
      closest = distToVert23;
      result = lineToVert23;
    }
    if (distToVert31 < closest)
      result = lineToVert31;
    return (result);
  }

  double ProjectRayOntoSphere(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 sphereOrigin, double sphereRadius)
  {
    glm::vec3 rayToSphere = sphereOrigin - rayOrigin;
    double distToCentre = glm::length(rayToSphere);
    //Project input ray against ray to sphere center
    double inRayOnRayToCentre = dot(rayToSphere, rayDirection);
    double d = sphereRadius*sphereRadius - (distToCentre*distToCentre - inRayOnRayToCentre*inRayOnRayToCentre);
    // No intersection occurred ?, return -1
    if (d < 0.0) return (-1.0f);
    // Otherwise return distance to the intersecting
    return (inRayOnRayToCentre - sqrt(d));
  }


  // Primitive Collision

  bool IsPointInSphere(glm::vec3 point, glm::vec3 sphereOrigin, double sphereRadius)
  {
    return (glm::length(point - sphereOrigin) <= sphereRadius);
  }

  bool IsPointInTriangle(glm::vec3 point, glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3)
  {
    double PI = 3.14159265358979323846264338327950288;
    double total_angles = 0.0f;
    glm::vec3 v1 = glm::normalize(point - vert1);
    glm::vec3 v2 = glm::normalize(point - vert2);
    glm::vec3 v3 = glm::normalize(point - vert3);
    total_angles += acos(dot(v1, v2));
    total_angles += acos(dot(v2, v3));
    total_angles += acos(dot(v3, v1));
    if (fabs(total_angles - 2 * PI) <= 0.005) // add to 180 degrees?
      return true;
    return false;
  }

  bool IsPointBehindPlane(glm::vec3 point, glm::vec3 planePosition, glm::vec3 planeNormal)
  {
    glm::vec3 dir = planePosition - point;
    double distFromPlane = glm::dot(dir, planeNormal);
    if (distFromPlane < -0.001f)
      return false; // in front
    else
      if (distFromPlane > 0.001f)
        return true; // behind
    return false; // right on top of
  }


  // Mesh Collision

  void PerformCollisionCheck(Collision &colPackage, OBJ &mesh)
  {
    // plane data
    glm::vec3 ver1, vert2, vert3;
    glm::vec3 planePosition;
    glm::vec3 planeNormal;
    glm::vec3 planeVert1, planeVert2;

    glm::vec3 sphereIntersectionPoint;
    glm::vec3 planeIntersectionPoint;
    glm::vec3 polyIntersectionPoint;

    glm::vec3 position = colPackage.position;
    glm::vec3 elipsoidRadius = colPackage.Elipsoid;
    glm::vec3 velocity = colPackage.velocity;

    glm::vec3 normalizedVelocity = glm::normalize(velocity);
    double distanceToTravel = glm::length(velocity);

    double distToPlaneIntersection;
    double distToEllipsoidIntersection;

    // loop through all polys
    for (int polyItr = 0; polyItr < mesh.faceCount; polyItr++)
    {
      // Project triangle into ellipsoid space
      ver1.x = mesh.polys[polyItr].verticies[0].x / elipsoidRadius.x;
      ver1.y = mesh.polys[polyItr].verticies[0].y / elipsoidRadius.y;
      ver1.z = mesh.polys[polyItr].verticies[0].z / elipsoidRadius.z;

      vert2.x = mesh.polys[polyItr].verticies[1].x / elipsoidRadius.x;
      vert2.y = mesh.polys[polyItr].verticies[1].y / elipsoidRadius.y;
      vert2.z = mesh.polys[polyItr].verticies[1].z / elipsoidRadius.z;

      vert3.x = mesh.polys[polyItr].verticies[2].x / elipsoidRadius.x;
      vert3.y = mesh.polys[polyItr].verticies[2].y / elipsoidRadius.y;
      vert3.z = mesh.polys[polyItr].verticies[2].z / elipsoidRadius.z;

      // Calculate the plane for this poly
      planePosition = ver1;
      planeVert1 = vert2 - ver1;
      planeVert2 = vert3 - ver1;
      planeNormal = glm::normalize(glm::cross(planeVert1, planeVert2));

      // calculate sphere-plane intersection
      sphereIntersectionPoint = position - planeNormal;
      if (IsPointBehindPlane(sphereIntersectionPoint, planePosition, planeNormal))
      {
        distToPlaneIntersection = ProjectRayOntoPlane(sphereIntersectionPoint, planeNormal, planePosition, planeNormal);
        planeIntersectionPoint.x = sphereIntersectionPoint.x + distToPlaneIntersection * planeNormal.x;
        planeIntersectionPoint.y = sphereIntersectionPoint.y + distToPlaneIntersection * planeNormal.y;
        planeIntersectionPoint.z = sphereIntersectionPoint.z + distToPlaneIntersection * planeNormal.z;
      }
      else
      {
        distToPlaneIntersection = ProjectRayOntoPlane(sphereIntersectionPoint, normalizedVelocity, planePosition, planeNormal);
        planeIntersectionPoint.x = sphereIntersectionPoint.x + distToPlaneIntersection * normalizedVelocity.x;
        planeIntersectionPoint.y = sphereIntersectionPoint.y + distToPlaneIntersection * normalizedVelocity.y;
        planeIntersectionPoint.z = sphereIntersectionPoint.z + distToPlaneIntersection * normalizedVelocity.z;
      }
      //These may be correct ( if the intersection point is within the original poly)
      polyIntersectionPoint = planeIntersectionPoint;
      distToEllipsoidIntersection = distToPlaneIntersection;

      if (!IsPointInTriangle(planeIntersectionPoint, ver1, vert2, vert3))
      {
        polyIntersectionPoint = ProjectPointOntoTriangle(ver1, vert2, vert3, planeIntersectionPoint);
        distToEllipsoidIntersection = ProjectRayOntoSphere(polyIntersectionPoint, -normalizedVelocity, position, 1.0f);
        if (distToEllipsoidIntersection > 0)
        {// calculate true sphere intersection point
          sphereIntersectionPoint.x = polyIntersectionPoint.x + distToEllipsoidIntersection * -normalizedVelocity.x;
          sphereIntersectionPoint.y = polyIntersectionPoint.y + distToEllipsoidIntersection * -normalizedVelocity.y;
          sphereIntersectionPoint.z = polyIntersectionPoint.z + distToEllipsoidIntersection * -normalizedVelocity.z;
        }
      }

      // are we jammed in the poly?
      if (IsPointInSphere(polyIntersectionPoint, position, 1.0f))
        colPackage.isStuck = true;

      // Calculate collision data
      if ((distToEllipsoidIntersection > 0) && (distToEllipsoidIntersection <= distanceToTravel))
      {
        if ((colPackage.collisionFound == false) || (distToEllipsoidIntersection < colPackage.collisionTvalue))
        {
          colPackage.collisionTvalue = distToEllipsoidIntersection;
          colPackage.ellipsoidIntersectionPoint = sphereIntersectionPoint;
          colPackage.PolygonIntersectionPoint = polyIntersectionPoint;
          colPackage.collisionFound = true;
        }
      }
    }	
  }


  // Collision Response

  glm::vec3 HandleCollisionResponse(glm::vec3 position, glm::vec3 velocity, OBJ &mesh)
  {
    if (glm::length(velocity) < MovementEpsilon)
      return position;

    glm::vec3 destinationPosition = position + velocity;

    // initialize the collision state
    WorldCollision.velocity = velocity;
    WorldCollision.position = position;
    WorldCollision.collisionFound = false;
    WorldCollision.isStuck = false;
    WorldCollision.collisionTvalue = -1;

    // perform sphere on mesh collision
    PerformCollisionCheck(WorldCollision, mesh);

    if (WorldCollision.collisionFound == false)
    { // no collision? great move to target
      position = position + glm::normalize(velocity) * (glm::length(velocity) - MovementEpsilon);
      WorldCollision.lastSafePosition = position;
      return position;
    }
    else
    {// ow dear, here we go..
      if (WorldCollision.isStuck) // did we get stuck?
        return WorldCollision.lastSafePosition;
      // Slide along this polygon by moving to the surface and making velocity perpendicular
      glm::vec3 newSourcePoint;
      if (WorldCollision.collisionTvalue >= MovementEpsilon)
      {
        newSourcePoint = WorldCollision.position + glm::normalize(velocity) * float(WorldCollision.collisionTvalue - MovementEpsilon);
      }
      else
      {
        newSourcePoint = WorldCollision.position;
      }

      glm::vec3 slidePlaneOrigin = WorldCollision.PolygonIntersectionPoint;
      glm::vec3 slidePlaneNormal = newSourcePoint - WorldCollision.PolygonIntersectionPoint;

      double distToPlane = ProjectRayOntoPlane(destinationPosition, slidePlaneNormal,
        slidePlaneOrigin, slidePlaneNormal);

      // Calculate a new destination point along the sliding plane
      glm::vec3 newDestinationPoint;
      newDestinationPoint.x = destinationPosition.x + distToPlane * slidePlaneNormal.x;
      newDestinationPoint.y = destinationPosition.y + distToPlane * slidePlaneNormal.y;
      newDestinationPoint.z = destinationPosition.z + distToPlane * slidePlaneNormal.z;

      // Calculate new velocity vector
      glm::vec3 newVelocityVector = newDestinationPoint - WorldCollision.PolygonIntersectionPoint;

      // Start again with new position and velocity
      WorldCollision.lastSafePosition = position;
      return HandleCollisionResponse(newSourcePoint, newVelocityVector, mesh);
    }
  }
}

bool PhysicsCollision(glm::vec3 position, glm::vec3 velocity, glm::vec3 ellipsoidRadius, OBJ &mesh)
{
  // project position and velocity into ellipsoid space
  position /= ellipsoidRadius;
  velocity /= ellipsoidRadius;

  if (glm::length(velocity) < MovementEpsilon)
    return false;

  WorldCollision.velocity = velocity;
  WorldCollision.position = position;
  WorldCollision.Elipsoid = ellipsoidRadius;
  WorldCollision.collisionFound = false;
  WorldCollision.isStuck = false;
  WorldCollision.collisionTvalue = -1;

  PerformCollisionCheck(WorldCollision, mesh);
  return WorldCollision.collisionFound;
}

glm::vec3 PhysicsMove(glm::vec3 position, glm::vec3 velocity, glm::vec3 ellipsoidRadius, OBJ &mesh)
{
  // project position and velocity into ellipsoid space
  glm::vec3 scaledPosition = position / ellipsoidRadius;
  glm::vec3 scaledVelocity = velocity / ellipsoidRadius;
  WorldCollision.Elipsoid = ellipsoidRadius;

  glm::vec3 finalPosition = HandleCollisionResponse(scaledPosition, scaledVelocity, mesh);

  // project back to world space
  finalPosition = finalPosition * ellipsoidRadius;

  return finalPosition;
}

#endif // Physics_h__