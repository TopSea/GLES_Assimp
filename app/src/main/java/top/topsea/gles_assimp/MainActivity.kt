package top.topsea.gles_assimp

import android.content.res.AssetManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.SeekBar
import android.widget.TextView
import top.topsea.gles_assimp.databinding.ActivityMainBinding
import top.topsea.gles_assimp.vws.ModelView


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private lateinit var mGLView: ModelView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val assetManager = assets
        val pathToInternalDir = filesDir.absolutePath

        // call the native constructors to create an object
        createObjectNative(assetManager, pathToInternalDir)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        mGLView = binding.modelView

        binding.seekBar.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                changeFrameRate(0.0006f * progress)
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
//                TODO("Not yet implemented")
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
//                TODO("Not yet implemented")
            }

        })
    }

    override fun onResume() {
        super.onResume()
        mGLView.onResume()
    }

    override fun onPause() {
        super.onPause()
        mGLView.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        deleteObjectNative()
    }

    private external fun createObjectNative(assetManager: AssetManager, pathToInternalDir: String)
    private external fun deleteObjectNative()
    private external fun changeFrameRate(frame: Float)
}