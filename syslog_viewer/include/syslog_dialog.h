#ifndef SYSLOG_DIALOG_H
#define SYSLOG_DIALOG_H

#include <gtkmm.h>
#include <memory>
#include <vector>
#include <mutex>
#include "syslog_message.h"
#include "udp_listener.h"

class SyslogDialog : public Gtk::Box {
public:
    SyslogDialog();
    virtual ~SyslogDialog();

    // Start/stop listening
    void start_listening();
    void stop_listening();
    bool is_listening() const;

    // Port configuration
    void set_port(int port);
    int get_port() const;

    // Clear messages
    void clear_messages();

    // Export messages
    void export_to_file(const std::string& filename);

protected:
    // UI Components
    Gtk::Box controls_box_;
    Gtk::Box filter_box_;
    Gtk::Box button_box_;

    Gtk::Label port_label_;
    Gtk::SpinButton port_spin_;
    Gtk::Button start_button_;
    Gtk::Button stop_button_;
    Gtk::Button clear_button_;
    Gtk::Button export_button_;

    Gtk::Label filter_label_;
    Gtk::Entry filter_entry_;
    Gtk::CheckButton emergency_check_;
    Gtk::CheckButton alert_check_;
    Gtk::CheckButton critical_check_;
    Gtk::CheckButton error_check_;
    Gtk::CheckButton warning_check_;
    Gtk::CheckButton notice_check_;
    Gtk::CheckButton info_check_;
    Gtk::CheckButton debug_check_;

    Gtk::ScrolledWindow scrolled_window_;
    Gtk::TreeView tree_view_;

    Gtk::Label status_label_;

    // Tree model columns
    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns() {
            add(timestamp);
            add(severity);
            add(facility);
            add(hostname);
            add(application);
            add(message);
            add(source_ip);
            add(severity_enum);
        }

        Gtk::TreeModelColumn<Glib::ustring> timestamp;
        Gtk::TreeModelColumn<Glib::ustring> severity;
        Gtk::TreeModelColumn<Glib::ustring> facility;
        Gtk::TreeModelColumn<Glib::ustring> hostname;
        Gtk::TreeModelColumn<Glib::ustring> application;
        Gtk::TreeModelColumn<Glib::ustring> message;
        Gtk::TreeModelColumn<Glib::ustring> source_ip;
        Gtk::TreeModelColumn<int> severity_enum; // Hidden, for filtering
    };

    ModelColumns columns_;
    Glib::RefPtr<Gtk::ListStore> tree_model_;
    Glib::RefPtr<Gtk::TreeModelFilter> filtered_model_;

    // UDP Listener
    std::unique_ptr<UdpListener> listener_;

    // Message storage
    std::vector<SyslogMessage> messages_;
    std::mutex messages_mutex_;

    // Signal handlers
    void on_start_clicked();
    void on_stop_clicked();
    void on_clear_clicked();
    void on_export_clicked();
    void on_filter_changed();

    // Message handling
    void on_message_received(const SyslogMessage& msg);
    void add_message_to_view(const SyslogMessage& msg);

    // Filtering
    bool filter_func(const Gtk::TreeModel::const_iterator& iter);

    // Update status
    void update_status();

    // Dispatcher for thread-safe UI updates
    Glib::Dispatcher message_dispatcher_;
    SyslogMessage pending_message_;
    std::mutex pending_mutex_;
};

#endif // SYSLOG_DIALOG_H
