/**
 * Title:        SKPointShape
 * Description:  Just a point, represented as a circle.
 */
   package sketcher;
   import java.awt.event.*;
   import java.awt.Cursor;
   import java.awt.Point;

   import java.awt.Graphics;
   import java.awt.Color;

   public class SKPointShape extends SKNormalShape
   {public static SKMainFrame frameMain;
   
      public SKPointShape(SKMainFrame frame, int id, int x, int y)
      {
         super(frame,id,"Point"+id,"Point",0,x,y);
         frameMain=frame;
         IconPath = "/images/pointicon.gif";
         reshape(x-10,y-10,20,20); //Subtract half of width and heigth so x,y is center
      }
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      
      {
         if (vPropList != null)
         { //Update properties
            frameMain.propFactory.freeProps(vPropList);
            vPropList.add(frameMain.propFactory.getNewProp("ID", Integer.toString(ID),0,false));
            vPropList.add(frameMain.propFactory.getNewProp("Type",ShapeType,0,false));
            vPropList.add(frameMain.propFactory.getNewProp("Name",Name,0));
            vPropList.add(frameMain.propFactory.getNewProp("Group", Integer.toString(GroupID),0,false));
         }
      
         if (vCurrentConstraints != null)
         { //Update constraints
            vCurrentConstraints.clear();
            ConstraintList.copyArrayTo(vCurrentConstraints);
         }
         super.updateMainShapeData(vPropList,vCurrentConstraints);         
      }
   
      public void paintComponent(Graphics g)
      {
         if (shouldDraw())
         {
            updateColors();
         
         //Paint shape
            g.setColor(bodyColor);
         
            g.fillOval(5,5,10,10); //Make the shape smaller than the region (so point is small, but has mouse buffer zone)
         
            if (bodyColor != outlineColor)
            {
               g.setColor(outlineColor);
               g.drawOval(5,5,10,10);
            }
         }
      }
   
      public int getNumSubShapes()
      {
         return 1;
      }
   
      public SKBaseShape getSubShape(int index)
      {
         switch (index)
         {
            case 1 : 
               return this;
            default: 
               return this;
         }
      }
   
      public void paintSpecial(Graphics g)
      {
         if (getSelectable().highestGroup != null)
         {
            g.setColor(Color.black);
            g.drawString( new Integer( getSelectable().highestGroup.ID ).toString(), getX()+18, getY()+18 );
         }
      }
   
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      {
         super.updateConstraintProps(con,vPropList);
      
         vPropList.add( frameMain.propFactory.getNewProp(Name,Name,1,false) );
      }
   
      public void updateColors()
      
      {
         SKBaseShape sh;
      
         if (isClone)
            sh = clonedShape;
         else
            sh = this;
         if (frameMain.selectedGroupShapes.indexOf(sh.getSelectable()) > -1)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.grpShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.grpShapeOutlineColor];
         }
         if (sh.Selected)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.selShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.selShapeOutlineColor];
         }
         else if (sh == frameMain.mouseOverItem)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.moShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.moShapeOutlineColor];
         }
         else if (sh.fixed)
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.fixShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.fixShapeOutlineColor];
         }
         else
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.shapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.shapeOutlineColor];
         }
      
      }
   
   //Should this shape be drawn?
   
      public boolean shouldDraw()
      
      {  
         return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
   
   
   }
