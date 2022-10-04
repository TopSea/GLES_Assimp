package top.topsea.gles_assimp

import android.app.Application
import android.util.Log
import com.tencent.mmkv.MMKV
import org.apache.commons.compress.archivers.zip.ZipArchiveInputStream
import java.io.File
import java.io.FileOutputStream

const val TAG = "TopSea:::"

class ModelApplication : Application() {
    private lateinit var kv: MMKV

    override fun onCreate() {
        super.onCreate()

        MMKV.initialize(this)
        kv = MMKV.defaultMMKV()
        if (!kv.decodeBool("gl_src_init", false)) {
            glSrcInit()
        }
    }

    private fun glSrcInit() {
        val model = "cowboy.zip"
        val internalDir = File(this.filesDir, "model/${model.substringBefore(".")}")
        internalDir.mkdirs()
        Log.d(TAG, "modelInit: ${internalDir.absolutePath}")
        val zis = ZipArchiveInputStream(assets.open(model))
        ZipUtil.unzipFile(zis, internalDir)

        val vShader = "vShader.src"
        val fShader = "fShader.src"

        val shaderDir = File(this.filesDir, "shader")
        shaderDir.mkdirs()

        val vShaderFile = File(shaderDir.absolutePath, vShader)
        val fShaderFile = File(shaderDir.absolutePath, fShader)

        if (vShaderFile.createNewFile()) {
            assets.open(vShader).copyTo(FileOutputStream(vShaderFile))
        }
        if (fShaderFile.createNewFile()) {
            assets.open(fShader).copyTo(FileOutputStream(fShaderFile))
        }
        assets.close()
        kv.encode("gl_src_init", true)
    }

    companion object {
        init {
            System.loadLibrary("gles_assimp")
        }
    }
}