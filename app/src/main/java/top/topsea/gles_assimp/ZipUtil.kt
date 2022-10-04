package top.topsea.gles_assimp

import android.util.Log
import org.apache.commons.compress.archivers.ArchiveEntry
import org.apache.commons.compress.archivers.ArchiveInputStream
import org.apache.commons.compress.utils.IOUtils
import java.io.File
import java.io.IOException
import java.nio.file.Files

object ZipUtil {

    fun unzipFile(aInputStream: ArchiveInputStream, modelDir: File) {
        var entry: ArchiveEntry? = aInputStream.nextEntry

        val path: String
        if (entry!!.isDirectory) { // 如果压缩文件被不是都被压缩在一个文件夹中，就创建一个压缩包名称的文件夹存放文件
            path = modelDir.parentFile!!.absolutePath
            modelDir.delete()
        } else {
            path = modelDir.absolutePath
        }

        while (entry != null) {
            if (!aInputStream.canReadEntryData(entry)) {
                Log.e(TAG, "unzipFile: failed to read file: ${entry.name}")
                continue
            }
            val f = File(path, entry.name)

            if (entry.isDirectory) {
                if (!f.isDirectory && !f.mkdirs()) {
                    throw IOException("failed to create directory: $f");
                }
            } else {
                val parent = f.parentFile!!
                if (!parent.isDirectory && !parent.mkdirs()) {
                    throw IOException("failed to create directory $parent");
                }
                val o = Files.newOutputStream(f.toPath())
                IOUtils.copy(aInputStream, o)
            }

            entry = aInputStream.nextEntry
        }
    }
}