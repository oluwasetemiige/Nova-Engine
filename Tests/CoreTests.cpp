// Dependency-free tests for NovaCore. Uses a fake Window/GuiHost so no GPU or display is needed.
#include "Nova/Core/Application.h"
#include "Nova/Core/Log.h"

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

static int g_failures = 0;

#define CHECK(expr)                                                              \
    do                                                                           \
    {                                                                            \
        if (!(expr))                                                             \
        {                                                                        \
            std::fprintf(stderr, "CHECK FAILED %s:%d: %s\n", __FILE__, __LINE__, #expr); \
            ++g_failures;                                                        \
        }                                                                        \
    } while (false)

namespace
{
    using namespace Nova;

    // Fake window: closes itself after `framesBeforeClose` frames; time advances 1/60 s per frame.
    class NullWindow final : public Window
    {
    public:
        explicit NullWindow(int framesBeforeClose) : m_Limit(framesBeforeClose) {}
        void OnUpdate() override { ++m_Frames; }
        std::uint32_t GetWidth() const override { return 640; }
        std::uint32_t GetHeight() const override { return 360; }
        bool ShouldClose() const override { return m_Frames >= m_Limit; }
        void SetVSync(bool) override {}
        double GetTime() const override { return m_Frames / 60.0; }
        void* GetNativeWindow() const override { return nullptr; }

    private:
        int m_Limit;
        int m_Frames = 0;
    };

    struct CountingGui final : GuiHost
    {
        int Begins = 0, Ends = 0;
        void BeginFrame() override { ++Begins; }
        void EndFrame() override { ++Ends; }
    };

    // Records lifecycle calls into a shared journal so ordering can be asserted.
    class RecordingLayer final : public Layer
    {
    public:
        RecordingLayer(std::string name, std::vector<std::string>& journal)
            : Layer(name), m_Journal(journal) {}
        void OnAttach() override { m_Journal.push_back(GetName() + ":attach"); }
        void OnDetach() override { m_Journal.push_back(GetName() + ":detach"); }
        void OnUpdate(float dt) override
        {
            m_Journal.push_back(GetName() + ":update");
            LastDt = dt;
            ++Updates;
        }
        void OnGuiRender() override { m_Journal.push_back(GetName() + ":gui"); }

        float LastDt = -1.0f;
        int Updates = 0;

    private:
        std::vector<std::string>& m_Journal;
    };

    void TestLayerOrderingAndShutdown()
    {
        std::vector<std::string> journal;
        {
            LayerStack stack;
            stack.PushLayer(CreateScope<RecordingLayer>("A", journal));
            stack.PushOverlay(CreateScope<RecordingLayer>("Overlay", journal));
            stack.PushLayer(CreateScope<RecordingLayer>("B", journal)); // must land below the overlay
            CHECK(stack.Size() == 3);

            std::vector<std::string> order;
            for (auto& l : stack) order.push_back(l->GetName());
            CHECK((order == std::vector<std::string>{"A", "B", "Overlay"}));
        }
        const std::vector<std::string> expected = {
            "A:attach", "Overlay:attach", "B:attach",
            "Overlay:detach", "B:detach", "A:detach"}; // detached top-down
        CHECK(journal == expected);
    }

    void TestApplicationLoop()
    {
        std::vector<std::string> journal;
        auto gui = CreateScope<CountingGui>();
        CountingGui* guiPtr = gui.get();

        Application app({"TestApp"}, CreateScope<NullWindow>(5));
        app.SetGuiHost(std::move(gui));
        auto* a = static_cast<RecordingLayer*>(app.PushLayer(CreateScope<RecordingLayer>("A", journal)));
        auto* top = static_cast<RecordingLayer*>(app.PushOverlay(CreateScope<RecordingLayer>("Top", journal)));

        CHECK(&Application::Get() == &app);
        app.Run();

        CHECK(app.GetFrameCount() == 5);
        CHECK(a->Updates == 5 && top->Updates == 5);
        CHECK(guiPtr->Begins == 5 && guiPtr->Ends == 5);
        CHECK(std::fabs(a->LastDt - 1.0f / 60.0f) < 1e-4f);
        CHECK(std::fabs(app.GetLastFrameTime() - 1.0f / 60.0f) < 1e-4f);

        // Within one frame: all updates (bottom->top), then all gui calls.
        CHECK(journal[2] == "A:update" && journal[3] == "Top:update");
        CHECK(journal[4] == "A:gui" && journal[5] == "Top:gui");
    }

    void TestCloseStopsLoop()
    {
        struct Closer final : Layer
        {
            void OnUpdate(float) override
            {
                if (++Count == 3) Application::Get().Close();
            }
            int Count = 0;
        };
        Application app({"CloseApp"}, CreateScope<NullWindow>(1000));
        auto* closer = static_cast<Closer*>(app.PushLayer(CreateScope<Closer>()));
        app.Run();
        CHECK(closer->Count == 3);
        CHECK(app.GetFrameCount() == 3); // Close() only sets a flag: the current frame finishes, the next never starts
    }

    void TestLogHistory()
    {
        Log::SetConsoleOutput(false);
        Log::Clear();
        Log::SetMaxEntries(3);

        const auto rev0 = Log::Revision();
        Log::Info("hello {}", 42);
        Log::Warn("w");
        Log::Error("e");
        Log::Trace("t");

        CHECK(Log::Revision() == rev0 + 4);
        const auto entries = Log::Entries();
        CHECK(entries.size() == 3); // oldest dropped
        CHECK(entries[0].Message == "w" && entries[0].Level == LogLevel::Warn);
        CHECK(entries[2].Message == "t" && entries[2].Level == LogLevel::Trace);

        Log::Clear();
        CHECK(Log::Entries().empty());
        Log::SetMaxEntries(1000);
        Log::SetConsoleOutput(true);
    }
} // namespace

int main()
{
    Nova::Log::SetConsoleOutput(false); // keep test output readable; TestLogHistory re-enables
    TestLayerOrderingAndShutdown();
    TestApplicationLoop();
    TestCloseStopsLoop();
    TestLogHistory();

    if (g_failures == 0)
        std::printf("All NovaCore tests passed.\n");
    else
        std::printf("%d check(s) failed.\n", g_failures);
    return g_failures == 0 ? 0 : 1;
}
