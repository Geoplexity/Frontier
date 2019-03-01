/**
 * Title:        SKPointSubShape
 * Description:  This is for objects that use point(s) (ie SKLineShape, etc)
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Color;

   public class SKCircleSubShape extends SKCircleShape
   
   {
      public SKBaseShape parentShape;
   
   
      public SKCircleSubShape(SKMainFrame frame, int id, int x, int y, SKBaseShape parent)
      
      {
         super(frame,id,x,y);
         center = new SKPointSubShape(frame,id+1,x,y,parent);
         parentShape = parent;
      }
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints, boolean move)
      
      {
         center.doMove(deltaX,deltaY,resolveConstraints,move);
      }
   
      public void paintSpecial(Graphics g)
      
      {
         //super.paintSpecial(g);
      
         if (parentShape.isPrimaryShape(this))
            parentShape.paintSpecial(g);
      
      
         Color thisColor;
         if (Selected)
            thisColor = Color.red;
         else if (this==frameMain.mouseOverItem)
            thisColor = Color.white;
         else
            thisColor = Color.blue;
         int r=(int)(radius*frameMain.lastScale);
      
       // System.out.println("circle "+frameMain.lastScale+" "+r+" "+radius);
         g.setColor(thisColor);
         g.drawOval(center.getX()+(center.getWidth()/2)-(int)(r),center.getY()+(center.getHeight()/2)-(int)(r),2*r,2*r);
      }
   
   
      public SKBaseShape getSelectable()
      
      {
         return parentShape.getSelectable();
      }
   
   
      public void repaint()
      
      {
         super.repaint();
      
      //This needs to call even if it is not the primary subshape for selection, etc
         parentShape.repaint();
      }
   }