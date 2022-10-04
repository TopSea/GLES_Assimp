package top.topsea.gles_assimp.vws

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.util.Log
import top.topsea.gles_assimp.TAG

/**
 * @ProjectName:    AssimpModelDemo
 * @Package:        top.topsea.assimpmodeldemo.vws
 * @Description:
 * @Author:         TopSea
 * @AboutAuthor:    https://github.com/TopSea
 * @CreateDate:     2022/8/5
 **/
class ModelView(context: Context, attrs: AttributeSet):
    GLSurfaceView(context, attrs) {

    private lateinit var mRenderer: ModelRenderer

    init {
        try {
            setEGLContextClientVersion(3)
            mRenderer = ModelRenderer()
            setRenderer(mRenderer)

            renderMode = RENDERMODE_CONTINUOUSLY
            Log.d(TAG, "MeGLSurfaceView init!")
        } catch (e: Exception) {
            Log.e(TAG, "Unable to create GLES context!", e)
        }
    }
}