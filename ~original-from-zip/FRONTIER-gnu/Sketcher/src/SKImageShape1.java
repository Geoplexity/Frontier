/**
 * Title:        SKImageShape
 * Description:  Shape for images
 */
/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

   package sketcher;

   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.Image;
   import java.awt.image.*;
   import java.awt.*;
   import java.awt.geom.*;
   import javax.swing.ImageIcon;
   import java.io.File;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKImageShape extends SKNormalShape
   {
      public  ImageIcon image;
      public  String    imageFile;
      public  MyImageObserver ob;
      public  SKPointSubShape pt1,pt2;
      public  SKCircleSubShape circle;
      public  Image m ;
   
      private float rot;
      public static final String[] ConType = new String[] {"Arbitrary","Pt1","Pt2"};
      public  float     rotation = 0;
      private float     scale = 1;
   
      public SKImageShape(SKMainFrame frame, int newID, int x, int y)
      {
         super(frame,newID,"Image"+newID,"Image",6,x,y);
      
         imageFile = new String(frame.HomeDir+"/images/redford.gif");
         image = new ImageIcon(imageFile);
      
         IconPath = "/images/imageicon.gif";
         reshape(x,y,(int)(image.getIconWidth()*2.83*scale),(int)(image.getIconHeight()*2.83*scale));
         pt1 = new SKPointSubShape(frame,newID+1,(int)(x+getWidth()),y,this);
         pt2 = new SKPointSubShape(frame,newID+2,(int)(x+getWidth()),y,this);
         rot = rotation;
      }
   
      public void severTies()
      
      {
         super.severTies();
         pt1 = null;
         circle = null;
      }
   
      public void updateMainShapeData(SKPropArray vPropList, SKConstraintArray vCurrentConstraints)
      {
         super.updateMainShapeData(vPropList,vCurrentConstraints);
      
         if (vPropList != null)
         { //Update properties
            frameMain.propFactory.freeProps(vPropList);
            vPropList.add(frameMain.propFactory.getNewProp("Scale", Float.toString( scale*100 ),1));
            vPropList.add(frameMain.propFactory.getNewProp("ImageFile", imageFile, 1));
            vPropList.add(frameMain.propFactory.getNewProp("Rotation", Float.toString(rotation),1));
         }
         if (vCurrentConstraints != null)
         { //Update constraints
            vCurrentConstraints.clear();
            ConstraintList.copyArrayTo(vCurrentConstraints);
         }
      
      }
      public boolean setShapeData(String PropName, Object PropData)
      
      {
         if (super.setShapeData(PropName,PropData))  
            return true;
      
         if (PropName=="Scale")
         {
            scale = Float.parseFloat(PropData.toString()) / 100;
            reshape(getX(),getY(),(int)(image.getIconWidth()*scale),(int)(image.getIconHeight()*scale));
            pt1.reshape(getCircleX()-10,getY()-10,pt1.getWidth(), pt1.getHeight());
            circle.center.reshape(getCircleX()-10,getCircleY()-10,circle.center.getWidth(), circle.center.getHeight());
         }
         else if (PropName=="ImageFile")
         {
            if (new File(PropData.toString()).exists())
            {
               imageFile = PropData.toString();
               image = new ImageIcon(imageFile);
               reshape(getX(),getY(),(int)(image.getIconWidth()*scale),(int)(image.getIconHeight()*scale));
               pt1.reshape(getCircleX()-10,getY()-10,pt1.getWidth(), pt1.getHeight());
               pt2.reshape(getCircleX()-10,getCircleY()-10,pt2.getWidth(), pt2.getHeight());
            }
         }
         else if (PropName=="Rotation")
         {
            rotation = Float.parseFloat(PropData.toString());
            repaint();
         }
         else 
            return false;
      
         return true;
      }
      public void repaint()
      
      
      {
         frameMain.RefreshShapeArea( (int)getX(),(int)getY(),(int)getX()+getWidth(),(int)getY()+getHeight() );
      }
      public void paintComponent(Graphics g)
      
      {
         if (shouldDraw())
         {
            updateColors();
            Graphics2D g1 = (Graphics2D)g;
            AffineTransform T = new AffineTransform();
            if(rot != rotation)
            {
               AffineTransform T1 = new AffineTransform();
               T1.setToRotation(rotation-rot,(int)(getX()+(getWidth()/2)), (int)(getY()+(getHeight()/2)));
               float[] src,dst;
               src = new float[2];
               dst = new float[2];             
               src[0]=pt1.getShapeX();
               src[1]=pt1.getShapeY();
               T1.transform(src,0,dst,0,1);
               pt1.doMove(((int)dst[0]-pt1.getShapeX()),((int)dst[1]-pt1.getShapeY()),false,true);
            }
            T.setToRotation(rotation,getWidth(),getHeight());
            g1.transform(T);
            g1.drawImage(image.getImage(),1,1,(int)(getX()+(getWidth()/2)), (int)(getY()+(getHeight()/2)),null);
            g1.setColor(outlineColor);
            g1.drawRect(0,0,getWidth()-1,getHeight()-1);
         } 
         if(rot != rotation)
            rot = rotation;
      
      }
   
   
      public void updateColors()
      
      
      {//System.out.println("entered update color");
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
         {bodyColor    = SKOptions.colorOptions[SKOptions.moShapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.moShapeOutlineColor];
         }
         else
         {
            bodyColor    = SKOptions.colorOptions[SKOptions.shapeColor];
            outlineColor = SKOptions.colorOptions[SKOptions.shapeOutlineColor];
         }
      }
   
   
      public int getNumSubShapes()
      {
         return 2;
      }
   
      public SKBaseShape getSubShape(int index)
      
      {
         switch (index)
         {
            case 1 : 
               return pt1;
            case 2:
               return circle;
            default: 
               return this;
         }
      }
   	   //Should this shape be drawn?
   
      public boolean shouldDraw()
      { 
         return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
   
      public void setSelected(boolean sel)
      {
         if (sel != Selected)
         {
            Selected = sel;
            pt1.setSelected(sel);
            pt2.setSelected(sel);
         }
      }
   
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         this.setLocation(getX()+deltaX, getY()+deltaY);
         //super.doMove(deltaX,deltaY,false, true);
         if (SKOptions.byteOptions[ SKOptions.simpleSolverMode ]>0 && resolveConstraints)
         {
            if(SKOptions.byteOptions[ SKOptions.simpleSolverVersion ] == 0)
               SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable(),new SKConstraintArray(3));
            else 
               SKSimpleSolver.ResolveSimpleConstraints(this.getSelectable());
         }
      
         pt1.doMove(deltaX,deltaY,false, true);
         pt2.doMove(deltaX,deltaY,resolveConstraints, true);
      }
   
   
      public boolean doHitTest(int x, int y)
      {
         return (this.getBounds()).intersects(x-3,y-3,6,6);
      }
   
      public int getNumConstraintPoints(SKBaseConstraint con)
      {
         return 2;
      }
      public SKPointShape getConstraintSubShape(SKBaseConstraint con, int mod)
      {
         SKPointShape p;
         switch (mod)
         {
            case 1: p=pt1;
               break;
            case 2: p=pt2;
               break;
            default: p=pt2;
               break;
         }
         return p;
      }
   
      public Point getConstraintPoint(SKBaseConstraint con, int mod)
      {
         Point pt = new Point();
      
         switch (mod)
         {
            case  1:  
               { //Point 1
                  pt.x=pt1.getShapeX();
                  pt.y=pt1.getShapeY();
               }
               break;
            case  2:  
               { //Point 2
                  pt.x=pt2.getShapeX();
                  pt.y=pt2.getShapeY();
               }
               break;
            default:  
               break;
         }
      
         return pt;
      }
   
   
      public void setConstraintPoint(SKBaseConstraint con, int mod, Point pt)
      
      {
         int x,y;
         switch (mod)
         {
            case 1: 
               x = pt.x-getWidth()-getX();
               y = pt.y-getY();
               doMove(x,y,false);
               break;
            case 2: 
               x = pt.x-getWidth()-getX();
               y = pt.y-getHeight()-getY();
               doMove(x,y,false);
         }
      }
      public int getConstraintMod(SKBaseShape sh)
      
      {
         if (sh == pt1)
            return 1;
         else if (sh == pt2)
            return 2;
         else
            return 0;
      }
   
   
      public void updateConstraintProps(SKBaseConstraint con, SKPropArray vPropList)
      
      {
         super.updateConstraintProps(con,vPropList);
      
         vPropList.add( frameMain.propFactory.getNewComboProp(Name,con.getConInfo(this,0),1,ConType) );
      }
   
   
      public int getConPropInfo(SKBaseConstraint con, String PropName, Object PropData)
      
      {
         if (PropData=="Pt1")
            return 1;
         else if (PropData=="Pt2")
            return 2;
         else
            return 0;
      }
      public boolean isPrimaryShape(SKBaseShape sh)
      
      
      {
         return (pt1==sh);
      }
   
   
      public int getCircleX()
      {
         return getX()+getWidth();
      }
      public int getCircleY()
      {
         return getY()+getHeight();
      }
      public void updateFromUTU(char vr, char mod, double deltaX, double deltaY, double value)
      
      
      {
         switch (mod)
         {
            case 'a' : pt1.updateFromUTU(vr,mod,deltaX,deltaY,value);
               break;
            case 'b' : pt2.updateFromUTU(vr,mod,deltaX,deltaY,value);
         }
      }
   
   
      public void writeAdditionalProps(DataOutputStream p) throws IOException
      {
         super.writeAdditionalProps(p);
      
         p.writeUTF( imageFile );
         p.writeFloat( scale );
         p.writeFloat(rotation);
      }
   
      public void readAdditionalProps(DataInputStream p) throws IOException
      {
         super.readAdditionalProps(p);
      
         imageFile = p.readUTF();
         scale = p.readFloat();
         rotation = p.readFloat();
      
         if (!(new File(imageFile).exists()))
            imageFile = frameMain.HomeDir+"/redford.gif";
      
         image = new ImageIcon(imageFile);
         reshape(getX(),getY(),(int)(image.getIconWidth()*scale),(int)(image.getIconHeight()*scale));
         pt1.reshape(getCircleX()-10,getY()-10,pt1.getWidth(), pt1.getHeight());
         circle.center.reshape(getCircleX()-10,getCircleY()-10,circle.center.getWidth(), circle.center.getHeight());
      }
   
      public void readAdditionalProps(DataInputStream p, int x, int y) throws IOException
      
      
      {
         super.readAdditionalProps(p);
         imageFile = "image"+ID;
         scale = p.readFloat();
         rotation = p.readFloat();
         pt1.readAdditionalProps(p,x,y);
         frameMain.repFirst=false;
         circle.readAdditionalProps(p,x,y);
         if (!(new File(imageFile).exists()))
            imageFile = frameMain.HomeDir+"/redford.gif";
      
         image = new ImageIcon(imageFile);
         reshape(getX(),getY(),(int)(image.getIconWidth()*scale),(int)(image.getIconHeight()*scale));
         pt1.reshape(getCircleX()-10,getY()-10,pt1.getWidth(), pt1.getHeight());
         pt2.reshape(getCircleX()-10,getCircleY()-10,pt2.getWidth(), pt2.getHeight());
      }
   
   
   
   
   
      public int bifurUpdate(float[] data, int startIdx, int addX, int addY, float scale)
      
      {
         startIdx = pt1.bifurUpdate(data,startIdx,addX,addY,scale);
         startIdx = pt2.bifurUpdate(data,startIdx,addX,addY,scale);
         return startIdx+1;
      }
   
   
      public int bifurDataCount()
      
      {
         return pt1.bifurDataCount() + pt2.bifurDataCount() ;
      }
   
   
      public void cloneShape(SKBaseShape sh)
      
      {
         super.cloneShape(sh);
      
         SKImageShape cImage = (SKImageShape)sh;
         cImage.scale = scale;
         cImage.rotation = rotation;
         pt1.cloneShape(cImage.pt1);
         pt2.cloneShape(cImage.pt2);
      }
   }

   class MyImageObserver implements ImageObserver
   {
   
      public boolean imageUpdate(Image img, int infoflags, int x, int y, int width, int height) 
      {
         return true;
      }
   }
