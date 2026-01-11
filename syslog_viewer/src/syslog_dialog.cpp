#include "syslog_dialog.h"
#include <fstream>
#include <iostream>

SyslogDialog::SyslogDialog()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL, 5),
      controls_box_(Gtk::ORIENTATION_HORIZONTAL, 5),
      filter_box_(Gtk::ORIENTATION_HORIZONTAL, 5),
      button_box_(Gtk::ORIENTATION_HORIZONTAL, 5),
      port_label_("Port:"),
      start_button_("Start"),
      stop_button_("Stop"),
      clear_button_("Clear"),
      export_button_("Export"),
      filter_label_("Filter:"),
      emergency_check_("EMERG"),
      alert_check_("ALERT"),
      critical_check_("CRIT"),
      error_check_("ERROR"),
      warning_check_("WARN"),
      notice_check_("NOTICE"),
      info_check_("INFO"),
      debug_check_("DEBUG"),
      status_label_("Not listening"),
      listener_(std::make_unique<UdpListener>(514)) {

    set_margin_top(5);
    set_margin_bottom(5);
    set_margin_start(5);
    set_margin_end(5);

    // Configure port spin button
    port_spin_.set_range(1, 65535);
    port_spin_.set_value(514);
    port_spin_.set_increments(1, 10);
    port_spin_.set_digits(0);

    // Set up controls box
    controls_box_.set_margin_bottom(5);
    controls_box_.pack_start(port_label_, Gtk::PACK_SHRINK);
    controls_box_.pack_start(port_spin_, Gtk::PACK_SHRINK);
    controls_box_.pack_start(start_button_, Gtk::PACK_SHRINK);
    controls_box_.pack_start(stop_button_, Gtk::PACK_SHRINK);
    controls_box_.pack_start(clear_button_, Gtk::PACK_SHRINK);
    controls_box_.pack_start(export_button_, Gtk::PACK_SHRINK);
    controls_box_.pack_start(status_label_, Gtk::PACK_EXPAND_WIDGET);

    // Set up filter box
    filter_box_.set_margin_bottom(5);
    filter_box_.pack_start(filter_label_, Gtk::PACK_SHRINK);
    filter_box_.pack_start(filter_entry_, Gtk::PACK_EXPAND_WIDGET);

    // Set up severity filter checkboxes
    emergency_check_.set_active(true);
    alert_check_.set_active(true);
    critical_check_.set_active(true);
    error_check_.set_active(true);
    warning_check_.set_active(true);
    notice_check_.set_active(true);
    info_check_.set_active(true);
    debug_check_.set_active(true);

    button_box_.set_margin_bottom(5);
    button_box_.pack_start(emergency_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(alert_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(critical_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(error_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(warning_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(notice_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(info_check_, Gtk::PACK_SHRINK);
    button_box_.pack_start(debug_check_, Gtk::PACK_SHRINK);

    // Create tree model
    tree_model_ = Gtk::ListStore::create(columns_);
    filtered_model_ = Gtk::TreeModelFilter::create(tree_model_);
    filtered_model_->set_visible_func(
        sigc::mem_fun(*this, &SyslogDialog::filter_func)
    );

    tree_view_.set_model(filtered_model_);

    // Add columns
    tree_view_.append_column("Timestamp", columns_.timestamp);
    tree_view_.append_column("Severity", columns_.severity);
    tree_view_.append_column("Facility", columns_.facility);
    tree_view_.append_column("Source IP", columns_.source_ip);
    tree_view_.append_column("Hostname", columns_.hostname);
    tree_view_.append_column("Application", columns_.application);
    tree_view_.append_column("Message", columns_.message);

    // Make columns resizable and sortable
    for (auto column : tree_view_.get_columns()) {
        column->set_resizable(true);
        column->set_sort_column(column->get_sort_column_id());
    }

    // Set up scrolled window
    scrolled_window_.add(tree_view_);
    scrolled_window_.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    // Pack everything
    pack_start(controls_box_, Gtk::PACK_SHRINK);
    pack_start(filter_box_, Gtk::PACK_SHRINK);
    pack_start(button_box_, Gtk::PACK_SHRINK);
    pack_start(scrolled_window_, Gtk::PACK_EXPAND_WIDGET);

    // Connect signals
    start_button_.signal_clicked().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_start_clicked)
    );
    stop_button_.signal_clicked().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_stop_clicked)
    );
    clear_button_.signal_clicked().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_clear_clicked)
    );
    export_button_.signal_clicked().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_export_clicked)
    );

    filter_entry_.signal_changed().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );

    emergency_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    alert_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    critical_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    error_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    warning_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    notice_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    info_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );
    debug_check_.signal_toggled().connect(
        sigc::mem_fun(*this, &SyslogDialog::on_filter_changed)
    );

    // Set up message dispatcher
    message_dispatcher_.connect(
        sigc::mem_fun(*this, &SyslogDialog::add_message_to_view)
    );

    // Initial button states
    stop_button_.set_sensitive(false);
    update_status();

    show_all_children();
}

SyslogDialog::~SyslogDialog() {
    stop_listening();
}

void SyslogDialog::start_listening() {
    if (listener_->is_listening()) {
        return;
    }

    int port = static_cast<int>(port_spin_.get_value());
    listener_->set_port(port);

    try {
        listener_->start([this](const SyslogMessage& msg) {
            on_message_received(msg);
        });

        start_button_.set_sensitive(false);
        stop_button_.set_sensitive(true);
        port_spin_.set_sensitive(false);
        update_status();
    } catch (const std::exception& e) {
        Gtk::MessageDialog dialog("Error starting listener: " + std::string(e.what()),
                                 false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dialog.run();
    }
}

void SyslogDialog::stop_listening() {
    listener_->stop();
    start_button_.set_sensitive(true);
    stop_button_.set_sensitive(false);
    port_spin_.set_sensitive(true);
    update_status();
}

bool SyslogDialog::is_listening() const {
    return listener_->is_listening();
}

void SyslogDialog::set_port(int port) {
    if (!listener_->is_listening()) {
        port_spin_.set_value(port);
        listener_->set_port(port);
    }
}

int SyslogDialog::get_port() const {
    return listener_->get_port();
}

void SyslogDialog::clear_messages() {
    std::lock_guard<std::mutex> lock(messages_mutex_);
    messages_.clear();
    tree_model_->clear();
    update_status();
}

void SyslogDialog::export_to_file(const std::string& filename) {
    std::lock_guard<std::mutex> lock(messages_mutex_);

    std::ofstream file(filename);
    if (!file) {
        return;
    }

    file << "Timestamp,Severity,Facility,Source IP,Hostname,Application,Message\n";

    for (const auto& msg : messages_) {
        file << msg.timestamp_string() << ","
             << msg.severity_string() << ","
             << msg.facility_string() << ","
             << msg.source_ip << ","
             << msg.hostname << ","
             << msg.application << ","
             << "\"" << msg.message << "\"\n";
    }
}

void SyslogDialog::on_start_clicked() {
    start_listening();
}

void SyslogDialog::on_stop_clicked() {
    stop_listening();
}

void SyslogDialog::on_clear_clicked() {
    clear_messages();
}

void SyslogDialog::on_export_clicked() {
    Gtk::FileChooserDialog dialog("Export to CSV",
                                 Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("Save", Gtk::RESPONSE_OK);

    auto filter_csv = Gtk::FileFilter::create();
    filter_csv->set_name("CSV files");
    filter_csv->add_pattern("*.csv");
    dialog.add_filter(filter_csv);

    int result = dialog.run();

    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        if (filename.find(".csv") == std::string::npos) {
            filename += ".csv";
        }
        export_to_file(filename);
    }
}

void SyslogDialog::on_filter_changed() {
    filtered_model_->refilter();
}

void SyslogDialog::on_message_received(const SyslogMessage& msg) {
    {
        std::lock_guard<std::mutex> lock(messages_mutex_);
        messages_.push_back(msg);
    }

    {
        std::lock_guard<std::mutex> lock(pending_mutex_);
        pending_message_ = msg;
    }

    message_dispatcher_.emit();
}

void SyslogDialog::add_message_to_view(const SyslogMessage& msg) {
    SyslogMessage local_msg;
    {
        std::lock_guard<std::mutex> lock(pending_mutex_);
        local_msg = pending_message_;
    }

    auto row = *(tree_model_->append());
    row[columns_.timestamp] = local_msg.timestamp_string();
    row[columns_.severity] = local_msg.severity_string();
    row[columns_.facility] = local_msg.facility_string();
    row[columns_.source_ip] = local_msg.source_ip;
    row[columns_.hostname] = local_msg.hostname;
    row[columns_.application] = local_msg.application;
    row[columns_.message] = local_msg.message;
    row[columns_.severity_enum] = static_cast<int>(local_msg.severity);

    // Auto-scroll to new message
    auto adj = scrolled_window_.get_vadjustment();
    adj->set_value(adj->get_upper() - adj->get_page_size());

    update_status();
}

bool SyslogDialog::filter_func(const Gtk::TreeModel::const_iterator& iter) {
    // Text filter
    std::string filter_text = filter_entry_.get_text();
    if (!filter_text.empty()) {
        Glib::ustring message = (*iter)[columns_.message];
        Glib::ustring hostname = (*iter)[columns_.hostname];
        Glib::ustring application = (*iter)[columns_.application];

        if (message.lowercase().find(filter_text) == Glib::ustring::npos &&
            hostname.lowercase().find(filter_text) == Glib::ustring::npos &&
            application.lowercase().find(filter_text) == Glib::ustring::npos) {
            return false;
        }
    }

    // Severity filter
    int severity = (*iter)[columns_.severity_enum];

    switch (severity) {
        case 0: return emergency_check_.get_active();
        case 1: return alert_check_.get_active();
        case 2: return critical_check_.get_active();
        case 3: return error_check_.get_active();
        case 4: return warning_check_.get_active();
        case 5: return notice_check_.get_active();
        case 6: return info_check_.get_active();
        case 7: return debug_check_.get_active();
        default: return true;
    }
}

void SyslogDialog::update_status() {
    std::lock_guard<std::mutex> lock(messages_mutex_);

    if (listener_->is_listening()) {
        status_label_.set_text("Listening on port " + std::to_string(listener_->get_port()) +
                              " (" + std::to_string(messages_.size()) + " messages)");
    } else {
        status_label_.set_text("Not listening (" + std::to_string(messages_.size()) + " messages)");
    }
}
