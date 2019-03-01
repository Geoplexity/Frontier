/**
 * Title:        SKParallelConstraint
 * Description:  Parallel Constraint
 */
   package sketcher;
   import java.awt.Graphics;
   import java.awt.Color;

   public class SKParallelConstraint extends SKBaseConstraint
   {
      public int count;
      public SKMainFrame  frame= null;
      public SKParallelConstraint(SKMainFrame frameMain, int idnum)
      {
         super(frameMain,idnum,"Parallel"+idnum,"Parallel",3);
         frame = frameMain;
         autoAddShapeProps = false;
         drawMode = 1;
      }
   
      public boolean setMainData(String PropName, Object PropData)
      {
         return super.setMainData(PropName,PropData);
      }
   
      public static boolean isAvailable(SKShapeArray selectedShapes)
      {
         return ( (selectedShapes.get(0) instanceof SKLineShape) && (selectedShapes.get(1) instanceof SKLineShape) );
      }
   
   
      public void doPaint(Graphics g)
      
      {
         g.setColor(Color.black);
         this.isDrawn=false;
         super.doPaint(g);
      }
   
   
      public void drawConstraint(Graphics g,int x1,int y1,int x2,int y2)
      
      
      { super.drawConstraint(g,x1,y1,x2,y2);
      
      }
   
   }