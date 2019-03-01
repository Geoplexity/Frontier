
package sketcher;

   import javax.media.j3d.*;
   import javax.vecmath.*;


   public abstract class NewShapes extends Shape3D
   {
   
      protected GeometryArray shape;
   
      protected Point3f[] p;                // Points
   
      protected Point3f[] verts;            // Polygons
   
      static final byte X = 0;
      static final byte Y = 1;
      static final byte Z = 2;
      static final byte W = 3;
   }


