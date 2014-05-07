/**
 * Title:        SKPointSubShape
 * Description:  This is for objects that use point(s) (ie SKLineShape, etc)
 */
package sketcher;
   import java.awt.Graphics;

   import com.sun.j3d.utils.universe.*;
   import com.sun.j3d.utils.geometry.*;
   import com.sun.j3d.utils.behaviors.mouse.*;
   import javax.media.j3d.*;
   import javax.vecmath.*;
   import javax.media.j3d.J3DGraphics2D;
   import com.sun.j3d.utils.image.TextureLoader;
   import com.sun.j3d.utils.behaviors.picking.*;

   public class SKPointSubShape3d extends SKPointShape3d
   {
      public SKBaseShape3d parentShape;
   
      public SKPointSubShape3d(SKMainFrame frame, int id, int x, int y, SKBaseShape3d parent)
      {
         super(frame,id,x,y,0);
         //this.setUserData(parent.Name);
         parentShape = parent;
      }
   
      public void paintSpecial(Graphics g)
      {
         super.paintSpecial(g);
      
         if (parentShape.isPrimaryShape(this))
            parentShape.paintSpecial(g);
      }
   
      public SKBaseShape3d getSelectable()
      {
         return parentShape.getSelectable();
      }
      public void doMove()
      {
         //System.out.println("111>>"+this+" "+this.vector);
         parentShape.doMove();
      }
      public void repaint()
      {
         super.repaint();
      //This needs to call even if it is not the primary subshape for selection, etc
         parentShape.repaint();
      }
   }
