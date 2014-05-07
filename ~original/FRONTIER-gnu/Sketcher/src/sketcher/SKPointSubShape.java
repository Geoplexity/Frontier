/**
 * Title:        SKPointSubShape
 * Description:  This is for objects that use point(s) (ie SKLineShape, etc)
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Point;

   public class SKPointSubShape extends SKPointShape
   {
      public SKBaseShape parentShape;
   
      public SKPointSubShape(SKMainFrame frame, int id, int x, int y, SKBaseShape parent)
      {
         super(frame,id,x,y,0);
         parentShape = parent;
      }
      public void cloneShape(SKBaseShape sh)
      {
         super.cloneShape(sh);
         SKBaseShape parent = ((SKPointSubShape)sh).parentShape;
         int x= parent.getX()-parentShape.getX()+getX();
         int y= parent.getY()-parentShape.getY()+getY();
         sh.doMove(x-sh.getX(),y-sh.getY(),false);
      }
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         if(!(parentShape instanceof SKImageShape))
            super.doMove(deltaX,deltaY,resolveConstraints);
         else parentShape.doMove(deltaX,deltaY,resolveConstraints);
      }
      /*public void doMove(int deltaX, int deltaY, boolean resolveConstraints, boolean image)
      {
         //System.out.println("pt sub shape "+this.getX()+" "+this.getY());
      
         super.doMove(deltaX,deltaY,false);
         //System.out.println("pt sub shape 1 "+this.getX()+" "+this.getY());
         if(parentShape instanceof SKImageShape)
         {
            SKImageShape sh = (SKImageShape)parentShape;
            if(this.ID == sh.pt1.ID)
            {
               sh.pt1x=this.getX();
               sh.pt1y=this.getY();
            }
            else
            {
               sh.pt2x=this.getX();
               sh.pt2y=this.getY();
            }
            super.doMove(-deltaX,-deltaY,false);
            //sh.adjustPosition(this);
         }
      
      }*/
      		/*
      public void setConstraintPoint(SKBaseConstraint con, int mod, Point pt)
      
      {
         System.out.println("setShapeCon1");
         if(parentShape instanceof SKImageShape)
         {
            int x = pt.x-parentShape.getShapeX();
            int y = pt.y-parentShape.getShapeY();
            parentShape.doMove(x,y,false);
            System.out.println("setShapeCon");
         }
         else super.setConstraintPoint(con,mod,pt);
      }*/
      public void paintSpecial(Graphics g)
      {
         super.paintSpecial(g);
      
         if (parentShape.isPrimaryShape(this))
            parentShape.paintSpecial(g);
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
