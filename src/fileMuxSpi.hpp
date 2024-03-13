
#include "FS.h"

#include <FreeRTOS.h>
#include <semphr.h>

class FileImpl;
typedef std::shared_ptr<FileImpl> FileImplPtr;

class FileMutSpi : public File {
    
public:
    FileMutSpi() : File() {
        if (sdSem == NULL) {
            sdSem = xSemaphoreCreateMutex();
            if (sdSem == NULL) {
                Serial.println("Failed to create sdSem mutex");
            }
        }
    }

    FileMutSpi(const fs::File& file) : File(file) {
        if (sdSem == NULL) {
            sdSem = xSemaphoreCreateMutex();
            if (sdSem == NULL) {
                Serial.println("Failed to create sdSem mutex");
            }
        }
    }

    size_t write(uint8_t byte) override {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        size_t res = File::write(byte);
        xSemaphoreGive(sdSem);
        return res;
    }
    size_t write(const uint8_t *buf, size_t size) override {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        size_t res = File::write(buf, size);
        xSemaphoreGive(sdSem);
        return res;
    }
    int available() override {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        int res = File::available();
        xSemaphoreGive(sdSem);
        return res;
    }
    int read() override {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        int res = File::read();
        xSemaphoreGive(sdSem);
        return res;
    }
    int peek() override {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        int res = File::peek();
        xSemaphoreGive(sdSem);
        return res;
    }
    void flush() override {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        File::flush();
        xSemaphoreGive(sdSem);
    }
    size_t read(uint8_t* buf, size_t size) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        size_t res = File::read(buf, size);
        xSemaphoreGive(sdSem);
        return res;
    }
    size_t readBytes(char *buffer, size_t length)
    {
        return read((uint8_t*)buffer, length);
    }

    bool seek(uint32_t pos, SeekMode mode) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        bool res = File::seek(pos, mode);
        xSemaphoreGive(sdSem);
        return res;
    }
    bool seek(uint32_t pos)
    {
        return seek(pos, SeekSet);
    }
    size_t position() const {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        size_t res = File::position();
        xSemaphoreGive(sdSem);
        return res;
    }
    size_t size() const {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        size_t res = File::size();
        xSemaphoreGive(sdSem);
        return res;
    }
    bool setBufferSize(size_t size) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        bool res = File::setBufferSize(size);
        xSemaphoreGive(sdSem);
        return res;
    }
    void close() {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        File::close();
        xSemaphoreGive(sdSem);
    }
    operator bool() const {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        bool res = File::operator bool();
        xSemaphoreGive(sdSem);
        return res;
    }
    time_t getLastWrite() {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        time_t res = File::getLastWrite();
        xSemaphoreGive(sdSem);
        return res;
    }
    const char* path() const {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        const char* res = File::path();
        xSemaphoreGive(sdSem);
        return res;
    }
    const char* name() const {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        const char* res = File::name();
        xSemaphoreGive(sdSem);
        return res;
    }

    boolean isDirectory(void) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        boolean res = File::isDirectory();
        xSemaphoreGive(sdSem);
        return res;
    }
    boolean seekDir(long position) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        boolean res = File::seekDir(position);
        xSemaphoreGive(sdSem);
        return res;
    }
    FileMutSpi openNextFile(const char* mode = FILE_READ) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        FileMutSpi res = FileMutSpi(File::openNextFile(mode));
        xSemaphoreGive(sdSem);
        return res;
    }
    String getNextFileName(void) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        String res = File::getNextFileName();
        xSemaphoreGive(sdSem);
        return res;
    }
    String getNextFileName(boolean *isDir) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        String res = File::getNextFileName(isDir);
        xSemaphoreGive(sdSem);
        return res;
    }
    void rewindDirectory(void) {
        xSemaphoreTake(sdSem, portMAX_DELAY);
        File::rewindDirectory();
        xSemaphoreGive(sdSem);
    }

    static void sdTakeSem() {
        if (FileMutSpi::sdSem != NULL) {
            xSemaphoreTake(FileMutSpi::sdSem, portMAX_DELAY);
        }
    }
    static void sdGiveSem() {
        if (FileMutSpi::sdSem != NULL) {
            xSemaphoreGive(FileMutSpi::sdSem);
        }
    }

private:
    static xSemaphoreHandle sdSem;
};