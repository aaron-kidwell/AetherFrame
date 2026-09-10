#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <Windows.h>
#pragma comment(lib, "windowsapp.lib")


int main(void) {
    winrt::init_apartment();

    using namespace winrt::Windows::Devices::Bluetooth::Advertisement;
    using namespace winrt::Windows::Storage::Streams;

    try {
        BluetoothLEAdvertisementPublisher publisher;
        BluetoothLEAdvertisementDataSection datasection;

        DataWriter writer;
        writer.WriteByte(0x06);
        writer.WriteByte(0x00);
        writer.WriteByte(0xAF);
        writer.WriteByte(0x00); // sequence number to reassemble data

        writer.WriteByte(0x41); // encoded c2 data

        auto buffer = writer.DetachBuffer();
        datasection.Data(buffer);
        datasection.DataType(0xFF);
        publisher.StatusChanged([](auto sender, auto args) {
            printf("Status changed: %d Error: %d\n",
                (int)args.Status(),
                (int)args.Error());
            });

        publisher.Advertisement().DataSections().Append(datasection);
        publisher.Start();
        printf("Status: %d\n", (int)publisher.Status());

        Sleep(INFINITE);
        publisher.Stop();
    }
    catch (winrt::hresult_error const& ex) {
        printf("Error: %ls\n", ex.message().c_str());
    }


    // lots left to do

    return 0;
}