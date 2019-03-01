   package sketcher;
   import javax.media.j3d.Node;
   import javax.media.j3d.Transform3D;
   import com.sun.j3d.utils.behaviors.mouse.*;

   public interface SimpleTranslatingCallback extends MouseBehaviorCallback{
   
      public void transformChanged(int type, Transform3D transform);
   }