package top.topsea.gles_assimp.vws

import android.opengl.GLSurfaceView
import android.util.Log
import top.topsea.gles_assimp.TAG
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * @ProjectName:    AssimpModelDemo
 * @Package:        top.topsea.assimpmodeldemo.vws
 * @Description:
 * @Author:         TopSea
 * @AboutAuthor:    https://github.com/TopSea
 * @CreateDate:     2022/8/5
 **/
class ModelRenderer : GLSurfaceView.Renderer {

    private external fun drawFrameNative()
    private external fun surfaceCreatedNative()
    private external fun surfaceChangedNative(width: Int, height: Int)

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        Log.d(TAG, "onSurfaceCreated: ")
        surfaceCreatedNative()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        Log.d(TAG, "onSurfaceChanged: ")
        surfaceChangedNative(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        drawFrameNative()
    }
}