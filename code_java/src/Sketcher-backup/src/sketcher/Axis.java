/**************************************************************/
/* Axis Class                                                 */
/* Written by Daniel Reis, a UCSC student under the           */
/* supervision of professor Alex Pang.                        */
/* email contact: danielmr@iname.com                          */
/**************************************************************/
package sketcher;


   import javax.media.j3d.*;
   import javax.vecmath.*;


   public class Axis extends NewShapes
   {
   
   //******************************************************//
   // Axis - Create axis
   //******************************************************//
      public Axis(byte axis, Appearance app, int dist)
      {
         p = new Point3f[2];
      
      
         switch(axis)
         {
            case 0 :
               p[0] = new Point3f(-100.0f,0,dist);
               p[1] = new Point3f(100.0f,0,dist);
               break;
            case 1 :
               p[0] = new Point3f(0,-100.0f,0);
               p[1] = new Point3f(0,100.0f,0);
               break;
            case 2 :
               p[0] = new Point3f(dist,0,-100.0f);
               p[1] = new Point3f(dist,0,100.0f);
               break;
         }
      
         Point3f[] verts_temp = {
            p[0], p[1]
         };
      
         verts = verts_temp;
      
         shape = new LineArray(2, LineArray.COORDINATES);
         shape.setCapability(LineArray.ALLOW_COORDINATE_WRITE);
         shape.setCoordinates(0, verts);
         this.setGeometry(shape);
         this.setAppearance(app);
      }
   }
