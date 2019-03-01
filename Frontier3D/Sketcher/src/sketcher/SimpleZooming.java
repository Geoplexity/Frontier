   package sketcher;
   import javax.media.j3d.*;
   import com.sun.j3d.utils.behaviors.picking.*;
   import javax.media.j3d.Transform3D;
   //import com.sun.j3d.utils.picking.behaviors.PickZoomBehavior;

   public class SimpleZooming extends PickZoomBehavior {
   
      protected SimpleZoomingCallback callback = null;
      protected int pickMode = PickObject.USE_BOUNDS;
      protected int nodeType = PickObject.SHAPE3D;
      protected Node pickedNode = null;
   
   
      public SimpleZooming(BranchGroup root, Canvas3D canvas, Bounds bounds) {
      
         super(root, canvas, bounds);
      
         this.setSchedulingBounds(bounds);
      }
   
   
      public SimpleZooming( BranchGroup root, Canvas3D canvas, Bounds bounds,
                        int mode )
      
      {
         super(root, canvas, bounds);
         pickMode = mode;
      
         this.setSchedulingBounds(bounds);
      }
   
   
      public SimpleZooming( BranchGroup root, Canvas3D canvas, Bounds bounds,
                        int mode, int type)
      
      {
         super(root, canvas, bounds);
         pickMode = mode;
         nodeType = type;
      
         this.setSchedulingBounds(bounds);
      }
   
   /*
      public void setNodeType(int type) { 
      
         nodeType = type; }
   
   */
      public void setupCallback(SimpleZoomingCallback callback) { 
      
         this.callback = callback; }
   
      public void updateScene(int x, int y) {
         super.updateScene(x,y);
         Node node = pickScene.pickNode(pickScene.pickClosest(x, y), nodeType);
         pickedNode = node;
         Transform3D t = new Transform3D();
         callback.transformChanged(nodeType,t );
      
      }
   
   /*   public Node getNode()
      
      {
         return pickedNode;
      }*/
   }
