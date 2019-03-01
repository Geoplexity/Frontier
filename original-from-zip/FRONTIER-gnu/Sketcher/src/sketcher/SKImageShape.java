/**
 * Title:        SKImageShape
 * Description:  Shape for images
 */
   package sketcher;

   import java.awt.Graphics;
   import java.awt.Graphics2D;
   import java.awt.geom.*;
   import java.awt.*;
   import javax.swing.ImageIcon;
   import java.io.File;
   import java.io.DataOutputStream;
   import java.io.DataInputStream;
   import java.io.IOException;

   public class SKImageShape extends SKNormalShape
   {
      public  SKMainFrame frameMain;
      public  ImageIcon image;
      public  String    imageFile;
      public  SKPointSubShape pt1,pt2;
      public  SKCircleSubShape circle;
      public  float     scale = 1;
      public  float     rotation = 0;
      public  int       pt1x=0,pt1y=0,pt2x=0,pt2y=0;
      public  static final String[] ConType = new String[] {"Arbitrary","Pt1","Pt2"};
      private float rot;
      private boolean draw=false;
      private float s=1;
      public SKImageShape(SKMainFrame frame, int newID, int x, int y)
      {
         super(frame,newID,"Image"+newID,"Image",6,x,y);
         frameMain = frame;
         s=frameMain.lastScale;
         imageFile = new String(frame.HomeDir+"/images/redford.gif");
         image = new ImageIcon(imageFile);
      
         IconPath = "/images/imageicon.gif";
         reshape(x-(int)(0.15*image.getIconWidth()*scale*s),y-(int)(0.15*image.getIconHeight()*scale*s),(int)(image.getIconWidth()*scale*s),(int)(image.getIconHeight()*scale*s));
         //reshape(x,y,(int)(image.getIconWidth()*scale),(int)(image.getIconHeight()*scale));
         pt1 = new SKPointSubShape(frame,newID+1,(int)(x+(getWidth()*0.7*s)),(int)(y+(getHeight()*s*0.7)),this);
         pt2 = new SKPointSubShape(frame,newID+2,(int)(x+(getWidth()*0.7*s)),(int)y,this);
         rot = rotation;
      }
      public void severTies()
      
      {
         super.severTies();
         pt1 = null;
         pt2 = null;
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
            reshape(getX()-(int)(0.15*image.getIconWidth()*scale*s),getY()-(int)(0.15*image.getIconHeight()*scale*s),(int)(image.getIconWidth()*scale*s),(int)(image.getIconHeight()*scale*s));
            pt1.setShapePoint(new Point((int)(getX()+(getWidth()*0.85*s)),(int)(getY()+(getHeight()*0.85*s))));
            pt2.setShapePoint(new Point((int)(getX()+(getWidth()*0.85*s)),(int)(getY()+(getHeight()*0.15*s))));
         
            repaint();
         }
         else if (PropName=="ImageFile")
         {
            if (new File(PropData.toString()).exists())
            {
               imageFile = PropData.toString();
               image = new ImageIcon(imageFile);
               reshape(getX()-(int)(0.15*image.getIconWidth()*scale*s),getY()-(int)(0.15*image.getIconHeight()*scale*s),(int)(image.getIconWidth()*scale*s),(int)(image.getIconHeight()*scale*s));
               pt1.setShapePoint(new Point((int)(getX()+(getWidth()*0.85*s)),(int)(getY()+(getHeight()*0.85*s))));
               pt2.setShapePoint(new Point((int)(getX()+(getWidth()*0.85*s)),(int)(getY()+(getHeight()*0.15*s))));
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
   
   
   
      public void repaint(Graphics g)
      {
         paintComponent(g);
      }
      public void paintComponent(Graphics g)
      
      {
         Point P1,P2;
         //System.out.println("image "+rotation+" "+rot);
         /*double dist = SKSimpleSolver.distance(pt1.getShapeX(),pt1.getShapeY(),pt2.getShapeY(),pt2.getShapeX());
         if((int)dist != (getHeight()*0.7))
         {
            scale = (float)(dist/(getHeight()*0.7));
            reshape((int)(pt1.getShapeX() - (getWidth()*0.85)), (int)(pt1.getShapeY() - (getHeight()*0.85)), (int)(getWidth()*scale), (int)(getHeight()*scale));
         }*/
         if (shouldDraw())
         {
            if(s!=frameMain.lastScale)
            { s=frameMain.lastScale;
               reshape(getX(),getY(),(int)(getWidth()*s),(int)(getHeight()*s));}
            g.drawRect(0,0,getWidth()-1,getHeight()-1);
            updateColors();
            Graphics2D g1 = (Graphics2D)g;
            AffineTransform T = new AffineTransform();
            AffineTransform T1 = new AffineTransform();
            T.setToRotation(rotation,(int)(0.5*getWidth()),(int)(0.5*getWidth()));
         
            g1.transform(T);
            g1.drawImage(image.getImage(),1,1,(int)(getWidth()-2),(int)(getHeight()-2),null);
            g1.setColor(outlineColor);
            g1.drawRect((int)(0.15*getWidth())-1,(int)(0.15*getHeight())-1,(int)((getWidth()*0.7)),(int)((getHeight()*0.7)));
         
            if((rot!=rotation) && (!draw))
            {
               T1.setToRotation(rotation-rot,(int)(getX()+0.5*getWidth()*s),(int)(getY()+0.5*getHeight()*s));
               P2 = new Point();
               T1.transform(new Point(pt2.getShapeX(),pt2.getShapeY()),P2);
               pt2.setShapePoint(P2);
               T1.transform(new Point(pt1.getShapeX(),pt1.getShapeY()),P2);
               pt1.setShapePoint(P2);
               rot = rotation;
            }
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
               return pt2;
            default: 
               return this;
         }
      }
   
      public void doMove(int deltaX, int deltaY, boolean resolveConstraints)
      {
         super.doMove(deltaX,deltaY,false);
         pt1.doMove(deltaX,deltaY,false, true);
         pt2.doMove(deltaX,deltaY,resolveConstraints, true);
      }
      public boolean shouldDraw()
      {
         if(draw)
            return true;
         else
            return (!frameMain.drawGroupRects || frameMain.selectedGroupShapes.indexOf(this) > -1);
      }
      public void adjustPosition(SKPointShape p)
      {
         double slope=0.0;
         if(p.ID == pt1.ID)
            slope = (double)(pt1y-pt2.getY()) / (double)(pt1x-pt2.getX());
         else
            slope = (double)(pt1.getY()-pt2y) / (double)(pt1.getX()-pt2x);
         double inc = java.lang.Math.atan(slope);
         rotation = -(float)inc;
         //System.out.println("adjust "+pt1.getX()+" "+pt1.getY()+" "+pt2.getX()+" "+pt2.getY()+" "+slope+" "+inc);
         //System.out.println("adjust "+rotation);
         draw = true;
         /*if(p.ID == pt2.ID)
         {
            reshape((int)((p.getX()-0.85*getWidth())-(0.15*getWidth())),(int)((p.getY()-0.85*getHeight())-(0.15*getHeight())),(int)(getWidth()*scale),(int)(getHeight()*scale));
         //pt1.setShapePoint(new Point((int)(getX()+(getWidth()*0.85)),(int)(getY()+(getHeight()*0.85))));
            pt2.setShapePoint(new Point((int)(getX()+(getWidth()*0.85)),(int)(getY()+(getHeight()*0.15))));
         }*/
         repaint();
         draw = false;
      }
      public boolean doHitTest(int x, int y)
      {
         Rectangle r = new Rectangle(getX(),getY(),getWidth(),getHeight());
         return (r.getBounds()).intersects(x-3,y-3,6,6);
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
      public SKPointShape getOtherPoint(SKPointShape pt)
      {
         return (pt.equals(pt1)) ? pt2 : pt1;
      }
   
      public SKPointShape getLeftPoint()
      {
         return (pt1.getX()<pt2.getX()) ? pt1 : pt2;
      }
   
      public SKPointShape getTopPoint()
      {
         return (pt1.getY()<pt2.getY()) ? pt1 : pt2;
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
               {
                  pt.x=(pt1.getShapeX()+pt2.getShapeX())/2;
                  pt.y=(pt1.getShapeY()+pt2.getShapeY())/2;
                  if(con instanceof SKDistanceConstraint) ((SKDistanceConstraint)con).perp = true;
               }  
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
               x = pt.x-(int)(0.85*getWidth())-getX();
               y = pt.y-(int)(0.85*getHeight())-getY();
               doMove(x,y,false);
               break;
            case 2: 
               x = pt.x-(int)(0.85*getWidth())-getX();
               y = pt.y-(int)(0.15*getHeight())-getY();
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
         pt1.writeAdditionalProps(p);
         pt2.writeAdditionalProps(p);
      
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
      
      //reshape(getX()-(int)(0.15*image.getIconWidth()*scale),getY()-(int)(0.15*image.getIconHeight()*scale),(int)(image.getIconWidth()*scale),(int)(image.getIconHeight()*scale));
         pt1.readAdditionalProps(p);
         pt2.readAdditionalProps(p);
         //pt1.setShapePoint(new Point((int)(getX()+(getWidth()*0.85)),(int)(getY()+(getHeight()*0.85))));
         //pt2.setShapePoint(new Point((int)(getX()+(getWidth()*0.85)),(int)(getY()+(getHeight()*0.15))));
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

